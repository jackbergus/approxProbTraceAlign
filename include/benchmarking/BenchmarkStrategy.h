//
// Created by giacomo on 20/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_BENCHMARKSTRATEGY_H
#define FUZZYSTRINGMATCHING2_BENCHMARKSTRATEGY_H

#include <distances/path/LevensteinPathSimilarity.h>
#include <benchmarking/rows/TimeBenchmarkRowEntry.h>
#include <benchmarking/rows/PrecisionBenchmarkRowEntry.h>
#include "ReadGraphRankingStruct.h"
#include "benchmarking/Ranking.h"
#include "benchmarking/BenchmarkSeed.h"

typedef Ranking<struct path_info>       PathRanking;
typedef Ranking<ReadGraphRankingStruct> GraphRanking;

size_t distinct_chars(const std::string& query);

struct BenchmarkStrategy {
    bool doNotVisitLoopsTwice;
    size_t maxPathLength;
    const double minimumPathCost;
    std::string dataset_name;
    std::string configuration;
    std::function<double(const path_info&, const path_info&)> path_string_similarity = [](const path_info& left, const path_info& right) {
        return GeneralizedLevensteinDistance(left.path, right.path);
    };
    std::function<double(const ReadGraphRankingStruct&, const ReadGraphRankingStruct&)> graph_sim_function;



    BenchmarkStrategy(std::string datasetName, std::string configuration, bool doNotVisitLoopsTwice,
                      size_t maxPathLength, const double minimumPathCost,
                      std::function<double(const ReadGraphRankingStruct &,
                                           const ReadGraphRankingStruct &)> graphSimFunction);


    void singleQueryBenchmark(const BenchmarkSeed& test, ReadGraph& rg);
    void singleQueryMultipleGraphsBenchmark(const BenchmarkSeed& test, const std::vector<ReadGraph>& graphCollection);

    virtual void    scorePath(const std::string& query,
                              ReadGraph* graph,
                              const struct path_info& path_from_graph,
                              PathRanking& pathRanking,
                              double& benchmark_time
                             ) = 0;

    virtual void    scoreGraph(const std::string &query,
                               ReadGraph *rg,
                               const std::string &graph_name,
                               Ranking<ReadGraphRankingStruct> &graphRanking,
                               double &benchmark_time) = 0;

    friend std::ostream &operator<<(std::ostream &os, const BenchmarkStrategy &strategy);

private:
    template <typename T>
    void finalizeTest(const BenchmarkSeed &test, Ranking<T> &rankQuery, Ranking<T> &expeQuery,
                      const std::function<double(const T &, const T &)> &f);

    PathRanking rankGraphPaths(const std::string& query,
                               ReadGraph& rg,
                               double& totalTime,
                               std::unordered_map<size_t, double>& benchmark_time_by_path_size,
                               std::unordered_map<size_t, double>& benchmark_time_by_distinct_node_size,
                               double& averageTime
                              );

    PathRanking doPartialRank(ReadGraph &rg,
                              const path_info &expected,
                              const std::string &query,
                              const std::string &configuration,
                              const case_precision_type &eSCase,
                              const case_precision_type &eRCase,
                              const case_precision_type &ePCase
                             );


    std::vector<TimeBenchmarkRowEntry> table_time_benchmarks;
    std::vector<PrecisionBenchmarkRowEntry> table_precision_benchmarks;
};

#endif //FUZZYSTRINGMATCHING2_BENCHMARKSTRATEGY_H
