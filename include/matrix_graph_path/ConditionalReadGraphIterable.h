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

private:
    /**
     * Generic path iterator generators
     * @param isBegin       The only distinction is between the begin and the end iterator
     * @return
     */
    PathIterator iterators(bool isBegin = true);
};


#endif //FUZZYSTRINGMATCHING2_CONDITIONALREADGRAPHITERABLE_H
