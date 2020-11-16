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
 * @param query                 Trace that we want to align within the graph
 * @param dulcior               Whether we want to return the ranking by position (dulcior=true) or by actual ranking values (dulcior=false)
 * @param scoringFunction       Function to determine the probability/similarity ranking
 * @return                      Expected ranking using the expected ranking function
 */
TopKRanking generateExampleExpectedRanking(size_t graph_i, std::unordered_map<std::pair<size_t, size_t>, struct path_info, pair_hash>& pathsInGraph, const std::string& query, double dulcior, ExpressionEvaluator* scoringFunction);


#endif //FUZZYSTRINGMATCHING2_FUNCTOR_RANKINGS_H
