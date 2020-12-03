/*
 * MultiplePathsEmbeddingStrategy.h
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


    std::vector<struct path_info> collectPaths(ReadGraph& rg);
    ReadGraph::path_to_uembedding generateUnstructuredEmbeddingsFromRawPaths(ReadGraph& rg, std::vector<struct path_info>& it);
    ReadGraph::path_to_uembedding operator()(ReadGraph& rg);
};


#endif //FUZZYSTRINGMATCHING2_MULTIPLEPATHSEMBEDDINGSTRATEGY_H
