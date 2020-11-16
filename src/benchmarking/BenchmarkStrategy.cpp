//
// Created by giacomo on 20/10/20.
//

#include <chrono>
#include <benchmarking/BenchmarkStrategy.h>
#include <benchmarking/RankPathCollection.h>
#include <benchmarking/RankPathCollectionPathInfo.h>
#include <benchmarking/RankPathCollectionReadGraph.h>
#include "benchmarking/rows/PrecisionBenchmarkRowEntry.h"


BenchmarkStrategy::BenchmarkStrategy(std::string datasetName, std::string configuration, bool doNotVisitLoopsTwice,
                                     size_t maxPathLength, const double minimumPathCost,
                                     std::function<double(const ReadGraphRankingStruct &,
                                                          const ReadGraphRankingStruct &)> graphSimFunction)
        : doNotVisitLoopsTwice(doNotVisitLoopsTwice), maxPathLength(maxPathLength),
          minimumPathCost(minimumPathCost), dataset_name(datasetName), configuration(configuration),
          graph_sim_function(graphSimFunction) {

}



void BenchmarkStrategy::singleQueryBenchmark(const BenchmarkSeed &test, ReadGraph &rg) {

    PathRanking  rankQuery = doPartialRank(rg, test.expected_result, test.query, "single_query" + this->configuration +" with_query " + test.query, SCORE_EXPECTED_FROM_QUERY_SET, RANK_EXPECTED_FROM_QUERY_SET, RANK_PRECISION_EXPECTED_FROM_QUERY_SET);
    PathRanking  expeQuery = doPartialRank(rg, test.expected_result, test.expected_result.path, "single_query" + this->configuration +" with_expected " + test.expected_result.path, SCORE_EXPECTED_FROM_EXPECTED_SET, RANK_EXPECTED_FROM_EXPECTED_SET, RANK_PRECISION_EXPECTED_FROM_EXPECTED_SET);


    finalizeTest(test, rankQuery, expeQuery, path_string_similarity);

    /**double setDistance = rankQuery.normalizedRank(expeQuery, f, f);
    double distanceFromExpectedCost = std::abs(setDistance - expected_score);
    table_precision_benchmarks.emplace_back(dataset_name, configuration, test.query.size(), distinct_chars(test.query), SET_DISTANCE, setDistance);
    table_precision_benchmarks.emplace_back(dataset_name, configuration, test.query.size(), distinct_chars(test.query),SET_DIFFERENCE, distanceFromExpectedCost);*/
}

void BenchmarkStrategy::singleQueryMultipleGraphsBenchmark(const BenchmarkSeed &test, const std::vector<ReadGraph> &graphCollection) {
    double totalTime = 0.0;
    double averageTime = 0.0;
    std::unordered_map<size_t, double> benchmark_time_by_path_size;
    std::unordered_map<size_t, double> benchmark_time_by_distinct_node_size;
    size_t query_length = test.query.size();
    size_t query_distinct_nodes = distinct_chars(test.query);

    RankPathCollectionReadGraph j;
    GraphRanking rankQuery, expeQuery;

    double avgExpectedScore = 0.0;
    double avgExpectedPrecision = 0.0;
    size_t expectedRank = -1;

    bool checkExpectedIsAGraph = false;
    ReadGraph* ptr;
    for (const ReadGraph& graphName : graphCollection) {
        if (graphName.name == test.expected_result.path) {
            checkExpectedIsAGraph = true;
            ptr = (ReadGraph*)&graphName;
            break;
        }
    }
    if (!checkExpectedIsAGraph) throw std::runtime_error("Error: the expected element within the BenchmarkSeed must now be a graph! No loaded graph with such name was provided");

    // Getting the by path ranking among all the graphs within the collection
    RankPathCollection<std::vector<ReadGraph>, ReadGraph, ReadGraphRankingStruct> iterator{test.query, nullptr, totalTime, benchmark_time_by_path_size, benchmark_time_by_distinct_node_size, averageTime, this, &j};
    rankQuery = iterator.rankGraphPaths(graphCollection);

    table_time_benchmarks.emplace_back(dataset_name, configuration, TOTAL_TIME, query_length, query_distinct_nodes, 0.0, totalTime);
    table_time_benchmarks.emplace_back(dataset_name, configuration, AVG_TIME, query_length, query_distinct_nodes, 0.0, averageTime);
    for (const auto& cp: benchmark_time_by_path_size) {
        table_time_benchmarks.emplace_back(dataset_name, configuration, PATH_SIZE, query_length, query_distinct_nodes, (double)cp.first, cp.second);
    }
    for (const auto& cp: benchmark_time_by_distinct_node_size) {
        table_time_benchmarks.emplace_back(dataset_name, configuration, N_DISTINCT_NODES, query_length, query_distinct_nodes, (double)cp.first, cp.second);
    }

    {
        // For the expected graph within the collection, retrieving the paths that satisfy the query
        RankPathCollectionPathInfo j2;
        double disposableTime1 = 0.0, disposableTime2 = 0.0;

        std::unordered_map<size_t, double> disposable1, disposable2;
        RankPathCollection<ConditionalReadGraphIterable, struct path_info, struct path_info> iterator3{test.query, ptr, disposableTime2, disposable1, disposable2, disposableTime1, this, &j2};
        Ranking<path_info> rank = iterator3.rankGraphPaths(ptr->iterateOverPaths(doNotVisitLoopsTwice, maxPathLength, minimumPathCost));
        const auto vector = rank.maximum()->second;

        for (const auto& alternativeQueries : vector) {

            double expectedScore     = rank.score(alternativeQueries, 0.0);
            double expectedPrecision = rank.getRankingPrecision(alternativeQueries);

            avgExpectedScore += expectedScore;
            avgExpectedPrecision += expectedPrecision;

            std::unordered_map<size_t, double> disposable3;
            std::unordered_map<size_t, double> disposable4;
            RankPathCollection<std::vector<ReadGraph>, ReadGraph, ReadGraphRankingStruct> iterator2{alternativeQueries.path, nullptr, disposableTime2, disposable3, disposable4, disposableTime1, this, &j};

            expeQuery.mergeRanks(iterator2.rankGraphPaths(graphCollection));
        }
        avgExpectedScore /= (double)vector.size();
        avgExpectedPrecision /= (double)vector.size();
        if (vector.size() == 1) {
            expectedRank = rank.getRanking(vector[0]);
        }
    }

    table_precision_benchmarks.emplace_back(dataset_name, configuration, query_length, query_distinct_nodes, SCORE_EXPECTED_FROM_EXPECTED_SET, avgExpectedScore);
    if (expectedRank != (size_t)-1) {
        table_precision_benchmarks.emplace_back(dataset_name, configuration, query_length, query_distinct_nodes, RANK_EXPECTED_FROM_EXPECTED_SET, expectedRank);
    }
    table_precision_benchmarks.emplace_back(dataset_name, configuration, query_length, query_distinct_nodes, RANK_PRECISION_EXPECTED_FROM_EXPECTED_SET, avgExpectedPrecision);

    double setDistance = rankQuery.normalizedRank(expeQuery, graph_sim_function);
    double distanceFromExpectedCost = std::abs(setDistance - test.expected_result.cost);

    table_precision_benchmarks.emplace_back(dataset_name, configuration, test.query.size(), distinct_chars(test.query), SET_DISTANCE, setDistance);
    table_precision_benchmarks.emplace_back(dataset_name, configuration, test.query.size(), distinct_chars(test.query),SET_DIFFERENCE, distanceFromExpectedCost);
}




PathRanking BenchmarkStrategy::doPartialRank(ReadGraph &rg, const path_info &expected, const std::string &query,
                                         const std::string &configuration, const case_precision_type &eSCase,
                                         const case_precision_type &eRCase, const case_precision_type &ePCase) {
    size_t query_length = query.size();
    size_t query_distinct_nodes = distinct_chars(query);

    std::unordered_map<size_t, double> benchmark_time_by_path_size;
    std::unordered_map<size_t, double> benchmark_time_by_distinctnodes_size;
    double avgTime = 0.0;
    double totalTime = 0.0;
    Ranking rank = rankGraphPaths(query, rg, totalTime, benchmark_time_by_path_size, benchmark_time_by_distinctnodes_size, avgTime);

    // ranking expected: if the path is returned, the distance between the expected result and the given query
    /*double expectedScore     = rank.score(expected, 0.0);
    double expectedRank      = rank.getRanking(expected);
    double expectedPrecision = rank.getRankingPrecision(expected);*/

    table_time_benchmarks.emplace_back(dataset_name, configuration, TOTAL_TIME, query_length, query_distinct_nodes, 0.0, totalTime);
    table_time_benchmarks.emplace_back(dataset_name, configuration, AVG_TIME, query_length, query_distinct_nodes, 0.0, avgTime);
    for (const auto& cp: benchmark_time_by_path_size) {
        table_time_benchmarks.emplace_back(dataset_name, configuration, PATH_SIZE, query_length, query_distinct_nodes, (double)cp.first, cp.second);
    }
    for (const auto& cp: benchmark_time_by_distinctnodes_size) {
        table_time_benchmarks.emplace_back(dataset_name, configuration, N_DISTINCT_NODES, query_length, query_distinct_nodes, (double)cp.first, cp.second);
    }

    /*table_precision_benchmarks.emplace_back(dataset_name, configuration, query_length, query_distinct_nodes, eSCase, expectedScore);
    table_precision_benchmarks.emplace_back(dataset_name, configuration, query_length, query_distinct_nodes, eRCase, expectedRank);
    table_precision_benchmarks.emplace_back(dataset_name, configuration, query_length, query_distinct_nodes, ePCase, expectedPrecision);*/

    return rank;
}

PathRanking  BenchmarkStrategy::rankGraphPaths(const std::string &query, ReadGraph &rg, double &totalTime,
                                          std::unordered_map<size_t, double> &benchmark_time_by_path_size,
                                          std::unordered_map<size_t, double> &benchmark_time_by_distinct_node_size,
                                          double &averageTime) {
    RankPathCollectionPathInfo j;
    RankPathCollection<ConditionalReadGraphIterable, struct path_info, struct path_info> iterator{query, &rg, totalTime, benchmark_time_by_path_size, benchmark_time_by_distinct_node_size, averageTime, this, &j};
    return iterator.rankGraphPaths(rg.iterateOverPaths(doNotVisitLoopsTwice, maxPathLength, minimumPathCost));
}

std::ostream &operator<<(std::ostream &os, const BenchmarkStrategy &strategy) {
    os << std::boolalpha;
    {
        auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        os << "Benchmark finished at " << std::ctime(&time) << std::endl;
    }
    os << " - Dataset: " << strategy.dataset_name << std::endl;
    os << " - Configuration: " << strategy.configuration << std::endl << std::endl;
    os << "Path Generation Details:" << std::endl;
    os << " * visiting paths with loops " << ((!strategy.doNotVisitLoopsTwice) ? "true" : "false") << std::endl;
    os << " * maximum path length (admitted) " << (strategy.maxPathLength) << std::endl;
    os << " * maximum path cost " << (strategy.minimumPathCost) << std::endl  << std::endl;

    os << "1) Time Benchmark (single path ranking): " << std::endl;
    {
        fort::char_table table;
        TimeBenchmarkRowEntry::init_header(table);
        for (const auto& time_benchmark_row : strategy.table_time_benchmarks) {
            table << time_benchmark_row;
        }
        os << table.to_string() << std::endl << std::endl;
    }

    os << "2) Similarity Benchmark (single path ranking): " << std::endl;
    {
        fort::char_table table;
        PrecisionBenchmarkRowEntry::init_header(table);
        for (const auto& time_benchmark_row : strategy.table_precision_benchmarks) {
            table << time_benchmark_row;
        }
        os << table.to_string() << std::endl << std::endl;
    }

    return os;
}

template<typename T>
void BenchmarkStrategy::finalizeTest(const BenchmarkSeed &test, Ranking<T> &rankQuery, Ranking<T> &expeQuery,
                                     const std::function<double(const T &, const T &)> &f) {
    if (test.expected_result.path != test.query) { // Performing the test only if the query is different from the expected result
        double setDistance = rankQuery.normalizedRank(expeQuery, f);
        //double distanceFromExpectedCost = std::abs(setDistance - expected_score);

        table_precision_benchmarks.emplace_back(dataset_name, configuration, test.query.size(), distinct_chars(test.query), SET_DISTANCE, setDistance);
        //table_precision_benchmarks.emplace_back(dataset_name, configuration, test.query.size(), distinct_chars(test.query),SET_DIFFERENCE, distanceFromExpectedCost);
    }
}



size_t distinct_chars(const std::string &query) {
    std::unordered_set<char> chunks;
    for (size_t i = 0, n = query.size(); i<n; i++) {
        chunks.insert(query[i]);
    }
    return chunks.size();
}

