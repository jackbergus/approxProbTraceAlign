/*
 * NodesWithTransitiveEdgeCost.h
 * This file is part of ProbabilisticTraceAlignment
 *
 * Copyright (C) 2020 - Giacomo Bergami
 *
 * ProbabilisticTraceAlignment is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * ProbabilisticTraceAlignment is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ProbabilisticTraceAlignment. If not, see <http://www.gnu.org/licenses/>.
 */


//
// Created by giacomo on 20/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_NODESWITHTRANSITIVEEDGECOST_H
#define FUZZYSTRINGMATCHING2_NODESWITHTRANSITIVEEDGECOST_H


#include <embeddings/labelled_paths/OnlyTransitiveEdgesCost.h>

template <bool use_new_definition = true>
struct NodesWithTransitiveEdgeCost : public OnlyTransitiveEdgesCost<use_new_definition> {
    std::unordered_map<std::string, double> currentNodeStep;
    std::unordered_map<std::string, double> node_embedding;
    double nodeSummation;
    double tuning;

    NodesWithTransitiveEdgeCost(const std::string& varepsilon, std::unordered_map<std::pair<std::string, std::string>, double, pair_hash> &e, double tuning_factor, double l,
                                size_t len = 1) : OnlyTransitiveEdgesCost<use_new_definition>{varepsilon, e, -1.0, l, len},
    tuning{tuning_factor},
    nodeSummation{0.0} {}

    ~NodesWithTransitiveEdgeCost() override {};

    void acceptNode(const std::string &node, double value) override  {
        assert(node != OnlyTransitiveEdgesCost<use_new_definition>::varepsilon);
        nodeSummation += (value );
        auto it = currentNodeStep.insert(std::make_pair(node, value));
        if (!it.second) it.first->second += value;
    }

    void nextNodeIteration(double thisNormalizationCost) override {
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

    void finalize(double weight) override {
        double edgeCost = tuning == -1.0 ? 1.0 : std::pow(tuning, OnlyTransitiveEdgesCost<use_new_definition>::len);
        double nodeCost = tuning == -1.0 ? 1.0 : edgeCost/*1-edgecost*/;
        OnlyTransitiveEdgesCost<use_new_definition>::finalize(weight * edgeCost);
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
                    OnlyTransitiveEdgesCost<use_new_definition>::pair_embedding.insert(std::make_pair(std::make_pair(OnlyTransitiveEdgesCost<use_new_definition>::varepsilon, it.first), weight_the_resulting_value_with_the_graph_s_weight)).second);
        }
    }
};


#endif //FUZZYSTRINGMATCHING2_NODESWITHTRANSITIVEEDGECOST_H
