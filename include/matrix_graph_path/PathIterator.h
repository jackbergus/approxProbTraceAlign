/*
 * PathIterator.h
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

#ifndef FUZZYSTRINGMATCHING2_PATHITERATOR_H
#define FUZZYSTRINGMATCHING2_PATHITERATOR_H

//#include <Generator.hpp>

#include <vector>
#include <cassert>
#include <sstream>
#include <Eigen/Sparse>


/**
 * From Knuth's the art of computer programming
 */
template  <typename T>  bool definitelyLessThan(T a, T b)
{
    return (b - a) > ( (std::abs(a) < std::abs(b) ? std::abs(b) : std::abs(a)) * std::numeric_limits<T>::epsilon());
}

/**
 * From Knuth's the art of computer programming
 */
template <typename T> bool approximatelyEqual(T a, T b)
{
    return std::abs(a - b) <= ( (std::abs(a) < std::abs(b) ? std::abs(b) : std::abs(a)) * std::numeric_limits<T>::epsilon());
}

/**
 * From Knuth's the art of computer programming
 */
template <typename T> bool approximatelyLessEqual(T a, T b)
{
    return definitelyLessThan(a, b) || approximatelyEqual(a, b);
}

#include <ostream>



#include <unordered_set>
#include <stack>
#include <iostream>
#include <matrix_graph_path/stack_info.h>

/**
 * The iterator interface, that is used for both the C++/iterable interface, as well as the java's. Both uses might
 * be used interchangeably
 *
 */
class PathIterator {
    using MatrixIt = Eigen::SparseMatrix<double, Eigen::RowMajor>::InnerIterator;

    Eigen::SparseMatrix<double, Eigen::RowMajor>* A;
    std::unordered_map<size_t, std::string>* inv_label_conversion;
    const double minimumPathCost;
    size_t maxPathLength;
    bool doNotVisitLoopsTwice;
    std::stack<stack_info> S;

public:




    PathIterator(const PathIterator& x);
    PathIterator(Eigen::SparseMatrix<double, Eigen::RowMajor> *a,
                 std::unordered_map<size_t, std::string> *invLabelConversion,
                 size_t maxPathLength,
                 const double minimumPathCost = 2.0 * std::numeric_limits<double>::epsilon(),
                 bool visitLoops = false);

    PathIterator& operator=(const PathIterator&) = default;

    /**
     * Initializes the iterator
     * @param currNode              Where to start the visit
     * @param destination           Where the visit should finish (all possible paths)
     * @param probabilisticPath     Initial cost associated to each possible path
     */
    void init(size_t currNode, size_t destination, double probabilisticPath = 1.0);

    /**
     * Java interface: returns whether there is something else to be returned
     * @return
     */
    bool hasNext();

    /**
     * Java interface: if something might be returned, returns the current element and moves forward
     * @return
     */
    path_info next();

    ~PathIterator() { clear(); }

    bool operator==(const PathIterator &rhs) const {
        return A == rhs.A &&
               inv_label_conversion == rhs.inv_label_conversion &&
               minimumPathCost == rhs.minimumPathCost &&
               maxPathLength == rhs.maxPathLength &&
               doNotVisitLoopsTwice == rhs.doNotVisitLoopsTwice &&
               S.size() == rhs.S.size();
    }

    bool operator!=(const PathIterator &rhs) const {
        return !(rhs == *this);
    }

    /**
     * C++/iterable interface: returning the current element
     * @return
     */
    path_info operator*();

    /**
     * C++/iterable interface: moving forward
     */
    void operator++();

private:
    void clear();
    bool calculateNext();
    inline void visitCurrentNeighbourAndMoveNext(const stack_info &topStackInformation, MatrixIt &currentIterationStep);
    inline bool canVisit(const stack_info &current, const Eigen::SparseMatrix<double, Eigen::RowMajor>::InnerIterator &it, size_t dst) const;
};


#endif //FUZZYSTRINGMATCHING2_PATHITERATOR_H
