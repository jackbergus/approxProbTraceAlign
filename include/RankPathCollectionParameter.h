//
// Created by giacomo on 21/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_RANKPATHCOLLECTIONPARAMETER_H
#define FUZZYSTRINGMATCHING2_RANKPATHCOLLECTIONPARAMETER_H

#if 0
#include <string>
#include <benchmarking/Ranking.h>
#include "ReadGraph.h"

template <typename Element, typename RankingArgument> struct RankPathCollectionParameter {
    virtual std::pair<size_t, size_t> runQuery(const Element &el, const std::string &query, Ranking<RankingArgument>& rank, double& benchmark_time, ReadGraph *rg, BenchmarkStrategy* strategy) = 0;
};

#endif //FUZZYSTRINGMATCHING2_RANKPATHCOLLECTIONPARAMETER_H
#endif