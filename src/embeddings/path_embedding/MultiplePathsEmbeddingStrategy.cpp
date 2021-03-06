/*
 * MultiplePathsEmbeddingStrategy.cpp
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

#include "embeddings/path_embedding/MultiplePathsEmbeddingStrategy.h"

ReadGraph::path_to_uembedding MultiplePathsEmbeddingStrategy::operator()(ReadGraph &rg) {
    auto it = rg.iterateOverPaths(doNotVisitLoopsTwice, maxPathLength, minimumPathCost);
    ReadGraph::path_to_uembedding result;
    for (const auto& path: it) {
        std::cerr << path << std::endl;
        result[path] = (generatePathEmbedding(rg, path));
    }
    return result;
}

std::vector<struct path_info> MultiplePathsEmbeddingStrategy::collectPaths(ReadGraph &rg) {
    std::vector<struct path_info> result;
    for (const auto& path: rg.iterateOverPaths(doNotVisitLoopsTwice, maxPathLength, minimumPathCost)) {
        if (!(result.size() % 100000)) {
            std::cout << result.size() << "... [" << path.path << "]" <<   std::flush;
        }
        result.emplace_back(path);
    }
    return result;
}

ReadGraph::path_to_uembedding MultiplePathsEmbeddingStrategy::generateUnstructuredEmbeddingsFromRawPaths(ReadGraph &rg,
                                                                                                         std::vector<struct path_info> &it) {
    ReadGraph::path_to_uembedding result;
    for (const auto& path: it) {
        result[path] = (generatePathEmbedding(rg, path));
    }
    return result;
}
