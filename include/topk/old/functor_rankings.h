//
// Created by giacomo on 16/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_FUNCTOR_RANKINGS_H
#define FUZZYSTRINGMATCHING2_FUNCTOR_RANKINGS_H


#include <topk/Ladenprogramm.h>
#include <unordered_map>

class ExpressionEvaluator; // Forward declaration to avoid integration bloat

/**
 * Generate the expected ranking using the scoring function that was preloaded by the configuration file
 * @param graph_i               id of the graph that we are considering
 * @param pathsInGraph          Set of the paths associated to the current graph, either approximated or exact.
 * @param caba                 Trace that we want to align within the graph
 * @param dulcior               Whether we want to return the ranking by position (dulcior=true) or by actual ranking values (dulcior=false)
 * @param probSimMetric       Function to determine the probability/similarity ranking
 * @return                      Expected ranking using the expected ranking function
 */
TopKRanking generateExampleExpectedRanking(std::unordered_map<std::pair<size_t, size_t>, struct path_info, pair_hash>& pathsInGraph, const std::string& caba, bool dulcior, ExpressionEvaluator* probSimMetric);



template <typename T> Ranking<T>
setRankingDulcior(const std::unordered_map<T, struct path_info, pair_hash> &pathsInGraph,
                  bool dulcior, const Ranking<T> &expectedRanking) {
    if (!dulcior) {
        return expectedRanking;
    } else {
        TopKRanking finalRanking;
        for (const auto& y: pathsInGraph) {
            finalRanking.addScore(y.first, expectedRanking.getRanking(y.first));
        }
        return finalRanking;
    }
}


#endif //FUZZYSTRINGMATCHING2_FUNCTOR_RANKINGS_H
