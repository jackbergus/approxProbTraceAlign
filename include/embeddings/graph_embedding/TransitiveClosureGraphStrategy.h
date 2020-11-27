/*
 * TransitiveClosureGraphStrategy.h
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
// Created by giacomo on 20/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_TRANSITIVECLOSUREGRAPHSTRATEGY_H
#define FUZZYSTRINGMATCHING2_TRANSITIVECLOSUREGRAPHSTRATEGY_H

//#include <ThomsonNFA.h>
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
        ///assert(max_length >= 0);
        T it{varepsilon, embedding, tuning, lambda, max_length};
        ///double tmp = 1.0;
        ///std::swap(tmp, rg.weight);
        double itWithMethodInvocation = 0.0;
        steady_clock::time_point queryTransformationToVectorTimeStart = steady_clock::now();
        for (const auto& path : rg.iterateOverPaths(false, maxPath, std::numeric_limits<double>::epsilon()*2)) {
            ///std::cout << path << std::endl;
            for (const auto& nodeId : path.actualPath) {
                auto x = rg.inv_label_conversion.at(nodeId);
                if (x != varepsilon) {
                    steady_clock::time_point queryTransformationToVectorTimeStart = steady_clock::now();
                    it.acceptNode(x, 1.0);
                    steady_clock::time_point queryTransformationToVectorTimeEnd = steady_clock::now();
                    double transformedQuery = duration_cast<std::chrono::nanoseconds>(queryTransformationToVectorTimeEnd - queryTransformationToVectorTimeStart).count()/1000000.0;
                    itWithMethodInvocation+= transformedQuery;
                }
            }
            steady_clock::time_point queryTransformationToVectorTimeStart = steady_clock::now();
            it.nextNodeIteration(1.0);
            steady_clock::time_point queryTransformationToVectorTimeEnd = steady_clock::now();
            double transformedQuery = duration_cast<std::chrono::nanoseconds>(queryTransformationToVectorTimeEnd - queryTransformationToVectorTimeStart).count()/1000000.0;
            itWithMethodInvocation+= transformedQuery;
        }
        ///std::swap(tmp, rg.weight);
        size_t i = 0;
        while ((current.nonZeros() > 0) && ((i++) != maxPath)) {
            matrix_iterator<T>(current, rg.inv_label_conversion, it);
            it.nextEdgeIteration();
            steady_clock::time_point queryTransformationToVectorTimeStart = steady_clock::now();
            steady_clock::time_point queryTransformationToVectorTimeEnd = steady_clock::now();
            double transformedQuery = duration_cast<std::chrono::nanoseconds>(queryTransformationToVectorTimeEnd - queryTransformationToVectorTimeStart).count()/1000000.0;
            itWithMethodInvocation+= transformedQuery;
            current = current * rg.A;
        }
        double totalTimeExtimation = it.finalize(rg.weight);
        double tareItExtimation = totalTimeExtimation - itWithMethodInvocation;
        steady_clock::time_point queryTransformationToVectorTimeEnd = steady_clock::now();
        double transformedQuery = duration_cast<std::chrono::nanoseconds>(queryTransformationToVectorTimeEnd - queryTransformationToVectorTimeStart).count()/1000000.0;
        benchmarking_time += (transformedQuery-tareItExtimation);
        return embedding;
    }

private:
    const double lambda;
    const double tuning;
    const size_t maxPath;
};


#endif //FUZZYSTRINGMATCHING2_TRANSITIVECLOSUREGRAPHSTRATEGY_H
