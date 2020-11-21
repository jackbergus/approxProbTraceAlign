/*
 * stack_info.cpp
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
// Created by giacomo on 19/10/20.
//

//#include <ThomsonNFA.h>
#include "matrix_graph_path/stack_info.h"

stack_info::stack_info(size_t currentNode, size_t destination, const std::vector<size_t> path, double probabilisticPath,
                       const std::unordered_set<size_t> &visited,
                       Eigen::SparseMatrix<double, Eigen::RowMajor>::InnerIterator currentIteration,
                       bool firstInsertion)
        : currentNode(currentNode), destination(destination), path((path)),
          probabilisticPath(probabilisticPath), visited(visited), currentIteration(currentIteration),
          firstInsertion(firstInsertion) {}

path_info stack_info::asPathInfo(const std::unordered_map<size_t, std::string> *map) const {
    path_info p;
    if (map) {
        std::stringstream  s;
        for (const auto& x : path) {
            const std::string y = map->at(x);
            if (y != ".") // removing the epsilon state
                s << map->at(x);
        }
        p.path = s.str();
    }
    p.actualPath = path;
    p.probability = probabilisticPath;
    return p;
}
