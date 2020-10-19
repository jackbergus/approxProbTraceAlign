//
// Created by giacomo on 20/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_NODESWITHTRANSITIVEEDGECOST_H
#define FUZZYSTRINGMATCHING2_NODESWITHTRANSITIVEEDGECOST_H


#include <embeddings/labelled_paths/OnlyTransitiveEdgesCost.h>

struct NodesWithTransitiveEdgeCost : public OnlyTransitiveEdgesCost {
    std::unordered_map<std::string, double> currentNodeStep;
    std::unordered_map<std::string, double> node_embedding;
    double nodeSummation = 0.0;

    NodesWithTransitiveEdgeCost(std::unordered_map<std::pair<std::string, std::string>, double, pair_hash> &e, double l,
                                size_t len);

    ~NodesWithTransitiveEdgeCost() override {};

    void acceptNode(const std::string &node, double value) override;

    void nextNodeIteration(double thisNormalizationCost) override;

    void finalize(double weight) override;
};


#endif //FUZZYSTRINGMATCHING2_NODESWITHTRANSITIVEEDGECOST_H
