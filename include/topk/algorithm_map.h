/*
 * algorithm_map.h
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
