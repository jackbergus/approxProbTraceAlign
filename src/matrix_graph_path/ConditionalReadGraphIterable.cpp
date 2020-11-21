/*
 * ConditionalReadGraphIterable.cpp
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

#include "matrix_graph_path/ConditionalReadGraphIterable.h"

ConditionalReadGraphIterable::ConditionalReadGraphIterable(size_t source, size_t target, double weight,
                                                           Eigen::SparseMatrix<double, Eigen::RowMajor> *A,
                                                           std::unordered_map<size_t, std::string> *inv_label_conversion,
                                                           size_t maxPathLength, const double minimumPathCost,
                                                           bool doNotVisitLoopsTwice)
        : maxPathLength(maxPathLength), minimumPathCost(minimumPathCost),
          doNotVisitLoopsTwice(doNotVisitLoopsTwice), source(source), target(target), weight(weight), A(A), inv_label_conversion(inv_label_conversion) {}

PathIterator ConditionalReadGraphIterable::begin() {
    PathIterator it = iterators(true);
    return it.hasNext() ? it : end(); // Returning the constructed iterator only if there are some elements to be returned
}

PathIterator ConditionalReadGraphIterable::end() {
    return iterators(false);
}

PathIterator ConditionalReadGraphIterable::iterators(bool isBegin) {
    PathIterator pv{A, inv_label_conversion, maxPathLength, minimumPathCost, doNotVisitLoopsTwice};
    if (isBegin) pv.init(source, target, weight);
    return pv;
}
