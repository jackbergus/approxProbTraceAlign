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
