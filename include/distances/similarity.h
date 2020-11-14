//
// Created by giacomo on 13/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_SIMILARITY_H
#define FUZZYSTRINGMATCHING2_SIMILARITY_H

template <class T> struct similarity;

#include <cmath>

template <class T, typename similarity_score> double normalized_similarity(const T& lhs, const T& rhs, const similarity_score& sim) {
    return sim(lhs, rhs)/std::sqrt(sim(lhs, lhs)*sim(rhs, rhs));
}

#endif //FUZZYSTRINGMATCHING2_SIMILARITY_H
