/*
 * EmbedPahtsOverSingleGraphStrategy.h
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

#ifndef FUZZYSTRINGMATCHING2_EMBEDPAHTSOVERSINGLEGRAPHSTRATEGY_H
#define FUZZYSTRINGMATCHING2_EMBEDPAHTSOVERSINGLEGRAPHSTRATEGY_H

#include "MultiplePathsEmbeddingStrategy.h"
#include <embeddings/graph_embedding/TransitiveClosureGraphStrategy.h>

/**
 * Uses the transitive closure strategy over the graph to generate multiple embeddings
 * @tparam T
 */
template <typename T>
struct EmbedPathsOverSingleGraphStrategy : public MultiplePathsEmbeddingStrategy {
    static_assert(
            std::is_base_of<LabelledPathVisitingStrategy, T>::value,
            "T must be a descendant of LabelledPathVisitingStrategy"
    );
    bool set_lambda_to_one;

    TransitiveClosureGraphStrategy<T> strategy;

    EmbedPathsOverSingleGraphStrategy(bool set_lambda_to_one, const std::string& varepsilon, double tuning_factor, double lambda, bool doNotVisitLoopsTwice, size_t maxPathLength, const double minimumPathCost)
            : MultiplePathsEmbeddingStrategy(varepsilon, doNotVisitLoopsTwice, maxPathLength, minimumPathCost), strategy{set_lambda_to_one, varepsilon, tuning_factor, lambda, maxPathLength}, set_lambda_to_one(set_lambda_to_one) {
    }

    ReadGraph::path_to_uembedding operator()(ReadGraph &rg) override {
        strategy.benchmarking_time = 0;
        return MultiplePathsEmbeddingStrategy::operator()(rg);
    }

    ReadGraph::unstructured_embedding generatePathEmbedding(ReadGraph &g, const path_info &path) override {
        ReadGraph pathGraph;
        steady_clock::time_point vpTreeTransformedStartQuery = steady_clock::now();
        double pathCost = ReadGraph::generateGraphFromPath(path.actualPath, pathGraph, g.inv_label_conversion, g.A);
        pathGraph.finalizeEdgesMatrix(g.weight * pathCost);
        steady_clock::time_point vpTreeTransformedEndQuery = steady_clock::now();
        double transformedQuerySearch = duration_cast<std::chrono::nanoseconds>(vpTreeTransformedEndQuery - vpTreeTransformedStartQuery).count()/1000000.0;
        ReadGraph::unstructured_embedding toReturn = strategy(pathGraph); //testStrategy
        benchmarking_cost += transformedQuerySearch;
        benchmarking_cost += strategy.benchmarking_time;
        return toReturn;
    }
};

#endif //FUZZYSTRINGMATCHING2_EMBEDPAHTSOVERSINGLEGRAPHSTRATEGY_H
