//
// Created by giacomo on 19/10/20.
//

#include <cassert>
#include "embeddings/labelled_paths/LabelledPathVisitingStrategy.h"

LabelledPathVisitingStrategy::LabelledPathVisitingStrategy(std::unordered_map<std::pair<std::string, std::string>, double, pair_hash> &E,
                                                           double l, size_t len) : pair_embedding{E}, lambda{l}, len{len}, lambda_pow{std::pow(lambda, (double)len)}, summation{0} {
    assert(std::abs(l)<=1.0);
}
