//
// Created by giacomo on 20/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_TRANSITIVECLOSUREGRAPHSTRATEGY_H
#define FUZZYSTRINGMATCHING2_TRANSITIVECLOSUREGRAPHSTRATEGY_H

#include <ThomsonNFA.h>
#include "GraphEmbeddingStrategy.h"

/**
 * Uses the transitive closure strategy over the graph to
 * @tparam T
 */
template <typename T>
struct TransitiveClosureGraphStrategy : public GraphEmbeddingStrategy {
    static_assert(
            std::is_base_of<LabelledPathVisitingStrategy, T>::value,
            "T must be a descendant of LabelledPathVisitingStrategy"
    );

    TransitiveClosureGraphStrategy(double lambda, size_t maxPath) : lambda(lambda), maxPath(maxPath) {}

    ReadGraph::unstructured_embedding operator()(ReadGraph &rg) override {
        Eigen::SparseMatrix<double, Eigen::RowMajor> current = rg.A;
        ReadGraph::unstructured_embedding embedding;
        T it{embedding, lambda};
        ///double tmp = 1.0;
        ///std::swap(tmp, rg.weight);
        for (const auto& path : rg.iterateOverPaths(true, maxPath, std::numeric_limits<double>::epsilon()*2)) {
            for (const auto& nodeId : path.actualPath) {
                if (rg.inv_label_conversion.at(nodeId) != EPSILON)
                    it.acceptNode(rg.inv_label_conversion.at(nodeId), 1.0);
            }
            it.nextNodeIteration(1.0);
        }
        ///std::swap(tmp, rg.weight);
        size_t i = 0;
        while ((current.nonZeros() > 0) && ((i++) != maxPath)) {
            matrix_iterator<T>(current, rg.inv_label_conversion, it);
            it.nextEdgeIteration();
            current = current * rg.A;
        }
        it.finalize(rg.weight);
        return embedding;
    }

private:
    const double lambda;
    const size_t maxPath;
};


#endif //FUZZYSTRINGMATCHING2_TRANSITIVECLOSUREGRAPHSTRATEGY_H
