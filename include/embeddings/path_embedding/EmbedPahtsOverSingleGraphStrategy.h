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

    TransitiveClosureGraphStrategy<T> strategy;

    EmbedPathsOverSingleGraphStrategy(double lambda, bool doNotVisitLoopsTwice, size_t maxPathLength, const double minimumPathCost)
            : MultiplePathsEmbeddingStrategy(doNotVisitLoopsTwice, maxPathLength, minimumPathCost), strategy{lambda} {
    }

    ReadGraph::unstructured_embedding generatePathEmbedding(ReadGraph &g, const path_info &path) override {
        ReadGraph pathGraph;
        double pathCost = ReadGraph::generateGraphFromPath(path.actualPath, pathGraph, g.inv_label_conversion, g.A);
        pathGraph.finalizeEdgesMatrix(g.weight * pathCost);
        return strategy(pathGraph);
    }
}

#endif //FUZZYSTRINGMATCHING2_EMBEDPAHTSOVERSINGLEGRAPHSTRATEGY_H
