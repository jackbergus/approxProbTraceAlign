/*
 * ConditionalReadGraphIterable.h
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

#ifndef FUZZYSTRINGMATCHING2_CONDITIONALREADGRAPHITERABLE_H
#define FUZZYSTRINGMATCHING2_CONDITIONALREADGRAPHITERABLE_H

#include <Eigen/Sparse>
#include <limits>
#include "PathIterator.h"


struct ConditionalReadGraphIterable {
    size_t maxPathLength;
    const double minimumPathCost = 2.0 * std::numeric_limits<double>::epsilon();
    bool doNotVisitLoopsTwice = true;
    size_t source;
    size_t target;
    double weight;
    Eigen::SparseMatrix<double, Eigen::RowMajor>* A;
    std::unordered_map<size_t, std::string>* inv_label_conversion;

    ConditionalReadGraphIterable(const ConditionalReadGraphIterable&) = default;
    ConditionalReadGraphIterable& operator=(const ConditionalReadGraphIterable&) = default;
    ConditionalReadGraphIterable(size_t source = 0, size_t target = 0, double weight = 1.0,
                                 Eigen::SparseMatrix<double, Eigen::RowMajor>* A = nullptr,
                                 std::unordered_map<size_t, std::string>* inv_label_conversion = nullptr,
                                 size_t maxPathLength = std::numeric_limits<size_t>::max(),
                                 const double minimumPathCost = 2.0 * std::numeric_limits<double>::epsilon(),
                                 bool doNotVisitLoopsTwice = true);


    PathIterator begin();
    PathIterator end();
    void resetParameters(size_t src, size_t tgt, Eigen::SparseMatrix<double, Eigen::RowMajor>* a,
                         std::unordered_map<size_t, std::string>* map) {
        source = src;
        target = tgt;
        A      = a;
        inv_label_conversion = map;
    }

private:
    /**
     * Generic path iterator generators
     * @param isBegin       The only distinction is between the begin and the end iterator
     * @return
     */
    PathIterator iterators(bool isBegin = true);
};


#endif //FUZZYSTRINGMATCHING2_CONDITIONALREADGRAPHITERABLE_H
