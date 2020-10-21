//
// Created by giacomo on 20/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_RANKPATHCOLLECTIONREADGRAPH_H
#define FUZZYSTRINGMATCHING2_RANKPATHCOLLECTIONREADGRAPH_H

#include <ReadGraph.h>
#include <benchmarking/RankPathCollection.h>

struct RankPathCollectionReadGraph : public RankPathCollectionParameter<ReadGraph, ReadGraphRankingStruct> {
    std::pair<size_t, size_t>
    runQuery(const ReadGraph &el, const std::string &query, GraphRanking &rank, double &benchmark_time, ReadGraph *rg,
             BenchmarkStrategy *strategy) override;

};


#endif //FUZZYSTRINGMATCHING2_RANKPATHCOLLECTIONREADGRAPH_H
