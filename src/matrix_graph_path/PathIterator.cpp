/*
 * PathIterator.cpp
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

#include "matrix_graph_path/PathIterator.h"

PathIterator::PathIterator(const PathIterator &x) : minimumPathCost{x.minimumPathCost} {
    A = x.A;
    inv_label_conversion = x.inv_label_conversion;
    maxPathLength = x.maxPathLength;
    doNotVisitLoopsTwice = x.doNotVisitLoopsTwice;
    S = x.S;
}

PathIterator::PathIterator(Eigen::SparseMatrix<double, Eigen::RowMajor> *a,
                           std::unordered_map<size_t, std::string> *invLabelConversion, size_t maxPathLength,
                           const double minimumPathCost, bool visitLoops)

        : A(a), inv_label_conversion(invLabelConversion),
          minimumPathCost(minimumPathCost), maxPathLength(maxPathLength == 0 ? a->size()*2 : maxPathLength),
          doNotVisitLoopsTwice(visitLoops), S{} {
    assert(a);
    assert(invLabelConversion);
}

void PathIterator::init(size_t currNode, size_t destination, double probabilisticPath) {
    clear();
    std::unordered_set<size_t> v;
    //v.emplace(currNode);
    Eigen::SparseMatrix<double, Eigen::RowMajor>::InnerIterator empty;
    S.push({currNode, destination, std::vector<size_t>{}, probabilisticPath, v, empty, true});
}

bool PathIterator::hasNext() {
    bool val = (!S.empty());
    if (val) val =
                     val&&calculateNext();
    if (!val) {
        clear();
    }
    return val;
}

path_info PathIterator::next() {
    path_info toRet;
    if (!S.empty()) {
        const auto x = S.top();
        toRet = x.asPathInfo(inv_label_conversion);
        ///if (x.currentNode == x.destination) S.pop();
        S.pop();
    }
    return toRet;
}

path_info PathIterator::operator*() {
    path_info toRet;
    if (!S.empty()) {
        const auto x = S.top();
        toRet = x.asPathInfo(inv_label_conversion);
    }
    return toRet;
}

void PathIterator::operator++() {
    S.pop();
    hasNext();
}

void PathIterator::clear() {
    while (! S.empty()) S.pop();
}

bool PathIterator::calculateNext() {
    while (! S.empty()) {
        stack_info& current = S.top();
        if (current.firstInsertion) {
            current.firstInsertion = false;
            if ((current.currentNode == current.destination) && (current.path.size() <= maxPathLength)) {
                current.path.emplace_back(current.currentNode);
                return true;
            } else {
                if ((!doNotVisitLoopsTwice) || (current.visited.emplace(current.currentNode).second)) {
                    current.currentIteration = MatrixIt(*A, current.currentNode);
                    visitCurrentNeighbourAndMoveNext(current, current.currentIteration);
                } else {
                    S.pop(); // Skip it: I cannot do more with it
                }
            }
        } else {
            if (!current.currentIteration) {
                S.pop(); // Skip it: I cannot do more with it
            } else {
                // Otherwise, continue to visit to the next node
                visitCurrentNeighbourAndMoveNext(current, current.currentIteration);
            }
        }

    }
    return false;
}

void PathIterator::visitCurrentNeighbourAndMoveNext(const stack_info &topStackInformation,
                                                    PathIterator::MatrixIt &currentIterationStep) {
    if (currentIterationStep) {
        size_t dst = currentIterationStep.col();
        if (canVisit(topStackInformation, currentIterationStep, dst)) {
            std::vector<size_t> v = topStackInformation.path;
            v.emplace_back(topStackInformation.currentNode);
            S.push({dst, topStackInformation.destination, v, topStackInformation.probabilisticPath * currentIterationStep.value(), topStackInformation.visited, {}, true});
        }
        ++currentIterationStep;
    }
}

bool
PathIterator::canVisit(const stack_info &current, const Eigen::SparseMatrix<double, Eigen::RowMajor>::InnerIterator &it,
                       size_t dst) const {
    return ( // Both the following conditions must follow:
                   (!doNotVisitLoopsTwice) || current.visited.find(dst) == current.visited.end()) && // 1) Both I can visit a node if and only if it has not visited before or if this condition is not required,
                  (current.path.size() <= maxPathLength) && //3) I forgot! we need to stop the iteration in the case that we already reach the maximum size limit!
                   (approximatelyLessEqual(minimumPathCost, current.probabilisticPath * it.value())); // 2) And the path cost must worth it

}
