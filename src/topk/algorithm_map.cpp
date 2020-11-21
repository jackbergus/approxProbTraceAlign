//
// Created by giacomo on 16/11/20.
//

#include "topk/algorithm_map.h"

algorithm_map::algorithm_map(size_t index, const std::vector<size_t> &edits, const std::vector<size_t> &sequence) :
    index(index),   edits(edits),   sequence(sequence) {}

algorithm_map::algorithm_map() {}
