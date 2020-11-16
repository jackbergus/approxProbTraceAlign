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
    std::string varepsilon;
    bool set_lambda_to_one;

    TransitiveClosureGraphStrategy(bool set_lambda_to_one, const std::string& varepsilon, double tuning_factor, double lambda, size_t maxPath) : tuning(tuning_factor), lambda(lambda), maxPath(maxPath), varepsilon(varepsilon), set_lambda_to_one(set_lambda_to_one) {}

    ReadGraph::unstructured_embedding operator()(ReadGraph &rg) override {
        Eigen::SparseMatrix<double, Eigen::RowMajor> current = rg.A;
        ReadGraph::unstructured_embedding embedding;
        int max_length = rg.max_length;
        if (set_lambda_to_one) {
            max_length = 1;
        } else if (max_length < 0) {
            max_length = maxPath;
        }
        assert(max_length >= 0);
        T it{varepsilon, embedding, tuning, lambda, max_length};
        ///double tmp = 1.0;
        ///std::swap(tmp, rg.weight);
        for (const auto& path : rg.iterateOverPaths(false, maxPath, std::numeric_limits<double>::epsilon()*2)) {
            std::cout << path << std::endl;
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
    const double tuning;
    const size_t maxPath;
};


#endif //FUZZYSTRINGMATCHING2_TRANSITIVECLOSUREGRAPHSTRATEGY_H
