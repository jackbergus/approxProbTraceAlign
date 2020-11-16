//
// Created by giacomo on 20/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_MULTIPLEPATHSEMBEDDINGSTRATEGY_H
#define FUZZYSTRINGMATCHING2_MULTIPLEPATHSEMBEDDINGSTRATEGY_H


#include <ReadGraph.h>

/**
 * Generating multiple embeddings for one single graph, where each embedding is associated to each single path
 */
struct MultiplePathsEmbeddingStrategy {
    bool doNotVisitLoopsTwice;
    size_t maxPathLength;
    const double minimumPathCost;
    std::string varepsilon;

    MultiplePathsEmbeddingStrategy(const std::string& varepsilon, bool doNotVisitLoopsTwice = true, size_t maxPathLength = std::numeric_limits<size_t>::max(),
                                       const double minimumPathCost = 2.0 * std::numeric_limits<double>::epsilon()) : doNotVisitLoopsTwice{doNotVisitLoopsTwice}, maxPathLength{maxPathLength}, minimumPathCost{minimumPathCost}, varepsilon{varepsilon} {}

    virtual ReadGraph::unstructured_embedding generatePathEmbedding(ReadGraph& rg, const path_info& path) = 0;

    ReadGraph::path_to_uembedding operator()(ReadGraph& rg);
};


#endif //FUZZYSTRINGMATCHING2_MULTIPLEPATHSEMBEDDINGSTRATEGY_H
