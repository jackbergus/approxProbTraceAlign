//
// Created by giacomo on 20/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_RANKPATHCOLLECTION_H
#define FUZZYSTRINGMATCHING2_RANKPATHCOLLECTION_H


#include "BenchmarkStrategy.h"
#include "RankPathCollectionParameter.h"

template <typename Iterator, typename Element, typename RankingArgument>
struct RankPathCollection {
    const std::string &query; ReadGraph *rg; double &totalTime;
    std::unordered_map<size_t, double> &benchmark_time_by_path_size;
    std::unordered_map<size_t, double> &benchmark_time_by_distinct_node_size;
    double &averageTime; BenchmarkStrategy* self; RankPathCollectionParameter<Element, RankingArgument>* ptr;

    RankPathCollection(const std::string &query, ReadGraph *rg, double &totalTime,
                       std::unordered_map<size_t, double> &benchmarkTimeByPathSize,
                       std::unordered_map<size_t, double> &benchmarkTimeByDistinctNodeSize, double &averageTime,
                       BenchmarkStrategy *self, RankPathCollectionParameter<Element, RankingArgument>* ptr) : query(query), rg(rg),
                                                  totalTime(totalTime), ptr(ptr),
                                                  benchmark_time_by_path_size(benchmarkTimeByPathSize),
                                                  benchmark_time_by_distinct_node_size(benchmarkTimeByDistinctNodeSize),
                                                  averageTime(averageTime), self(self) {}

    Ranking<RankingArgument> rankGraphPaths(Iterator it)  {
        assert(benchmark_time_by_path_size.empty());
        totalTime = 0.0;
        double nPaths = 0.0;
        Ranking<RankingArgument> rank;
        {
            std::unordered_map<size_t, std::vector<double>> benchmark_time_by_distinctNodes_disjoint;
            {
                std::unordered_map<size_t, std::vector<double>> benchmark_time_by_path_size_disjoint;
                for (const Element& path : it) {
                    double benchmark_time = 0.0;
                    const auto cp = ptr->runQuery(path, query, rank, benchmark_time, rg, self);

                    benchmark_time_by_path_size_disjoint[cp.first].emplace_back(benchmark_time);//path.actualPath.size()
                    benchmark_time_by_distinctNodes_disjoint[cp.second].emplace_back(benchmark_time);//distinct_chars(dc)
                    benchmark_time += benchmark_time;
                    nPaths += 1.0;
                }
                for (const auto& cp: benchmark_time_by_path_size_disjoint) {
                    benchmark_time_by_path_size_disjoint.emplace(cp.first, std::accumulate(cp.second.begin(), cp.second.end(), 0.0) / ((double)cp.second.size()));
                }
            }
            for (const auto& cp: benchmark_time_by_distinctNodes_disjoint) {
                benchmark_time_by_distinct_node_size.emplace(cp.first, std::accumulate(cp.second.begin(), cp.second.end(), 0.0) / ((double)cp.second.size()));
            }
        }
        averageTime = totalTime / nPaths;
    }

};

#endif //FUZZYSTRINGMATCHING2_RANKPATHCOLLECTION_H
