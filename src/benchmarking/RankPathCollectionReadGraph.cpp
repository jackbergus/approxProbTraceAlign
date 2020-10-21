//
// Created by giacomo on 20/10/20.
//

#include "benchmarking/RankPathCollectionReadGraph.h"

std::pair<size_t, size_t>
RankPathCollectionReadGraph::runQuery(const ReadGraph &el, const std::string &query, GraphRanking  &rank,
                                      double &benchmark_time, ReadGraph *rg, BenchmarkStrategy *strategy) {
    strategy->scoreGraph(query, (ReadGraph*)&el, el.name, rank, benchmark_time);
    std::unordered_set<std::string> labels;
    for (const auto& x : el.inv_label_conversion) {
        labels.emplace(x.second);
    }
    return {el.inv_label_conversion.size(), labels.size()};
}
