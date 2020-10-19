//
// Created by giacomo on 20/10/20.
//

#include "embeddings/path_embedding/MultiplePathsEmbeddingStrategy.h"

ReadGraph::path_to_uembedding MultiplePathsEmbeddingStrategy::operator()(ReadGraph &rg) {
    auto it = rg.iterateOverPaths(doNotVisitLoopsTwice, maxPathLength, minimumPathCost);
    ReadGraph::path_to_uembedding result;
    for (const auto& path: it) {
        result[path.path].emplace_back(generatePathEmbedding(rg, path));
    }
    return result;
}
