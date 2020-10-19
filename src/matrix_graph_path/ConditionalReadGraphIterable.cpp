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
