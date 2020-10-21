//
// Created by giacomo on 20/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_RANKPATHCOLLECTIONPATHINFO_H
#define FUZZYSTRINGMATCHING2_RANKPATHCOLLECTIONPATHINFO_H

#include <benchmarking/RankPathCollection.h>

struct RankPathCollectionPathInfo : public RankPathCollectionParameter<struct path_info, struct path_info> {
    std::pair<size_t, size_t>
    runQuery(const path_info &el, const std::string &query, PathRanking &rank, double &benchmark_time, ReadGraph *rg,
             BenchmarkStrategy *strategy) override;

};


#endif //FUZZYSTRINGMATCHING2_RANKPATHCOLLECTIONPATHINFO_H
