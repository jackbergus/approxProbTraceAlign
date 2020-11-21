/*
 * stack_info.h
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

#ifndef FUZZYSTRINGMATCHING2_STACK_INFO_H
#define FUZZYSTRINGMATCHING2_STACK_INFO_H

#include <vector>
#include <unordered_set>
#include <Eigen/Sparse>
#include "path_info.h"

// Iteration aspects
struct stack_info {
    size_t currentNode;
    size_t destination;
    std::vector<size_t> path;
    double probabilisticPath;
    std::unordered_set<size_t> visited;
    Eigen::SparseMatrix<double, Eigen::RowMajor>::InnerIterator currentIteration;
    bool firstInsertion;

    stack_info() = default;
    stack_info(size_t currentNode, size_t destination, const std::vector<size_t> path, double probabilisticPath,
               const std::unordered_set<size_t> &visited,
               Eigen::SparseMatrix<double, Eigen::RowMajor>::InnerIterator currentIteration, bool firstInsertion);
    stack_info(const stack_info& x) = default;
    stack_info& operator=(const stack_info&) = default;
    ~stack_info() = default;

    path_info asPathInfo(
            const std::unordered_map<size_t, std::string>* map) const;
};

#endif //FUZZYSTRINGMATCHING2_STACK_INFO_H
