//
// Created by giacomo on 20/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_GRAPHEMBEDDINGSTRATEGY_H
#define FUZZYSTRINGMATCHING2_GRAPHEMBEDDINGSTRATEGY_H

#include <ReadGraph.h>

struct GraphEmbeddingStrategy {
    virtual ReadGraph::unstructured_embedding operator()(ReadGraph &rg) = 0;
};

#endif //FUZZYSTRINGMATCHING2_GRAPHEMBEDDINGSTRATEGY_H
