//
// Created by giacomo on 19/10/20.
//

#include <ThomsonNFA.h>
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
            if (y != EPSILON) // removing the epsilon state
                s << map->at(x);
        }
        p.path = s.str();
    }
    p.actualPath = path;
    p.probability = probabilisticPath;
    return p;
}
