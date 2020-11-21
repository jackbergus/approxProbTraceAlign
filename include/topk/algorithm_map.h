//
// Created by giacomo on 16/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_ALGORITHM_MAP_H
#define FUZZYSTRINGMATCHING2_ALGORITHM_MAP_H

#include <vector>

struct algorithm_map {
    size_t index;
    std::vector<size_t> edits;
    std::vector<size_t> sequence;
    double precision = 0.0;
    double simialrity = 0.0;
    double score = 0.0;

    algorithm_map(const algorithm_map&) = default;
    algorithm_map& operator=(const algorithm_map& ) = default;
    algorithm_map(size_t index, const std::vector<size_t> &edits, const std::vector<size_t> &sequence);;
    algorithm_map();
};


#endif //FUZZYSTRINGMATCHING2_ALGORITHM_MAP_H
