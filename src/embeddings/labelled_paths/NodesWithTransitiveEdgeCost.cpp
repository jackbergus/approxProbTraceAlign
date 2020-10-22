//
// Created by giacomo on 20/10/20.
//

#include <ThomsonNFA.h>
#include "embeddings/labelled_paths/NodesWithTransitiveEdgeCost.h"

NodesWithTransitiveEdgeCost::NodesWithTransitiveEdgeCost(
        std::unordered_map<std::pair<std::string, std::string>, double, pair_hash> &e, double l, size_t len)
        : OnlyTransitiveEdgesCost{e, l, len},
        nodeSummation{0.0} {}

void NodesWithTransitiveEdgeCost::acceptNode(const std::string &node, double value) {
    assert(node != EPSILON);
    nodeSummation += (value );
    auto it = currentNodeStep.insert(std::make_pair(node, value));
    if (!it.second) it.first->second += value;
}

void NodesWithTransitiveEdgeCost::finalize(double weight) {
    double edgeCost = std::pow(lambda, len);
    double nodeCost = 1-edgeCost;
    OnlyTransitiveEdgesCost::finalize(weight * edgeCost);
    double S = 0; // Define a probability distribution over the all components: we need to normalize their values. Summing up all the results
    for (const auto& it : node_embedding) {
        S += (it.second * it.second);
    }
    S = std::sqrt(S);
    if (S <= std::numeric_limits<double>::epsilon())
        S = std::numeric_limits<double>::epsilon();
    for (auto& it : node_embedding) {
        const double normalized_over_current_length_distribution = (it.second / S); // Normalization of the component
        const double weight_the_resulting_value_with_the_graph_s_weight = normalized_over_current_length_distribution * nodeCost; // Multiplying by weight, so that if all the elegible criteria are met, the desired probability is returned
        assert(
                pair_embedding.insert(std::make_pair(std::make_pair(EPSILON, it.first), weight_the_resulting_value_with_the_graph_s_weight)).second);
    }
}

void NodesWithTransitiveEdgeCost::nextNodeIteration(double thisNormalizationCost) {
    for (auto& it : currentNodeStep) {
        const std::string& cp = it.first;
        double tmp = (it.second / (nodeSummation)) * thisNormalizationCost;
        auto final = node_embedding.insert(std::make_pair(cp, tmp)); // try to insert the current value
        if (!final.second) {
            final.first->second += tmp; // If a previous value exist, add this probability to the other one that was previously set up.
        }
    }
    nodeSummation = 0;
}
