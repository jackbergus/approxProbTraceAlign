//
// Created by giacomo on 15/10/20.
//

#include <cassert>
#include <cmath>
#include "Iterator.h"

iterator::iterator(std::unordered_map<std::pair<std::string, std::string>, double, pair_hash> &E, double l,
                              size_t len) : embedding{E} {
    assert(std::abs(l)<=1.0);
    lambda = l;
    this->len = len;
    lambda_pow = std::pow(lambda, (double)this->len);
    summation = 0;
}

#include <iostream>

void iterator::accept(const std::string &left, const std::string &right, double value) {
    summation += value;  // Summing up all the probabilities for the current iteration.
    auto it = current_step.insert(std::make_pair(std::make_pair(left, right), value));
    if (!it.second) it.first->second += value;
}

void iterator::nextIteration() {
    for (auto& it : current_step) {
        const std::pair<std::string, std::string>& cp = it.first;
        double tmp = (it.second / summation) * lambda_pow;
        auto final = embedding.insert(std::make_pair(cp, tmp)); // try to insert the current value
        if (!final.second) {
            final.first->second += tmp; // If a previous value exist, add this probability to the other one that was previously set up.
        }
    }
    current_step.clear();
    len++;
    lambda_pow *= lambda;
    summation = 0;
}

void iterator::finalize(double weight) {
    double S = 0; // Define a probability distribution over the all components: we need to normalize their values. Summing up all the results
    for (const auto& it : embedding) {
        S += it.second;
    }
    for (auto& it : embedding) {
        const double normalized_over_current_length_distribution = (it.second / S); // Normalization of the component
        const double weight_the_resulting_value_with_the_graph_s_weight = normalized_over_current_length_distribution * weight; // Multiplying by weight, so that if all the elegible criteria are met, the desired probability is returned
        it.second = weight_the_resulting_value_with_the_graph_s_weight;
    }
}