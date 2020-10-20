//
// Created by giacomo on 15/10/20.
//

#include <cassert>
#include <cmath>
#include "embeddings/labelled_paths/OnlyTransitiveEdgesCost.h"

OnlyTransitiveEdgesCost::OnlyTransitiveEdgesCost(std::unordered_map<std::pair<std::string, std::string>, double, pair_hash> &E, double l,
                                                 size_t len) : LabelledPathVisitingStrategy{E, l, len}, edge_summation{0.0} {
    /*assert(std::abs(l)<=1.0);
    lambda = l;
    this->len = len;
    lambda_pow = std::pow(lambda, (double)this->len);
    summation = 0;*/
}

#include <iostream>

void OnlyTransitiveEdgesCost::acceptMultiedge(const std::string &left, const std::string &right, double value) {
    edge_summation += value;  // Summing up all the probabilities for the current iteration.
    auto it = currentEdgeStep.insert(std::make_pair(std::make_pair(left, right), value));
    if (!it.second) it.first->second += value;
}

void OnlyTransitiveEdgesCost::nextEdgeIteration() {
    for (auto& it : currentEdgeStep) {
        const std::pair<std::string, std::string>& cp = it.first;
        double tmp = (it.second / edge_summation) * lambda_pow;
        auto final = pair_embedding.insert(std::make_pair(cp, tmp)); // try to insert the current value
        if (!final.second) {
            final.first->second += tmp; // If a previous value exist, add this probability to the other one that was previously set up.
        }
    }
    currentEdgeStep.clear();
    len++;
    lambda_pow *= lambda;
    edge_summation = 0;
}

void OnlyTransitiveEdgesCost::finalize(double weight) {
    double S = 0; // Define a probability distribution over the all components: we need to normalize their values. Summing up all the results
    for (const auto& it : pair_embedding) {
        S += it.second;
    }
    for (auto& it : pair_embedding) {
        const double normalized_over_current_length_distribution = (it.second / S); // Normalization of the component
        const double weight_the_resulting_value_with_the_graph_s_weight = normalized_over_current_length_distribution * weight; // Multiplying by weight, so that if all the elegible criteria are met, the desired probability is returned
        it.second = weight_the_resulting_value_with_the_graph_s_weight;
    }
}
