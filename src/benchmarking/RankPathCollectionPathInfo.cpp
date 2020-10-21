//
// Created by giacomo on 20/10/20.
//

#include "benchmarking/RankPathCollectionPathInfo.h"

std::pair<size_t, size_t>
RankPathCollectionPathInfo::runQuery(const path_info &el, const std::string &query, PathRanking &rank,
                                     double &benchmark_time, ReadGraph *rg, BenchmarkStrategy *strategy) {
    assert(rg);
    assert(strategy);
    strategy->scorePath(el.path, rg, el, rank, benchmark_time);
    return {el.path.size(), distinct_chars(el.path)};
}
