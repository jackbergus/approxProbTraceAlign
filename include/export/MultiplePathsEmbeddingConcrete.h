/*
 * MultiplePathsEmbeddingConcrete.h
 * This file is part of ProbabilisticTraceAlignment
 *
 * Copyright (C) 2021 - Giacomo Bergami
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
// Created by giacomo on 19/03/21.
//

#ifndef FUZZYSTRINGMATCHING2_MULTIPLEPATHSEMBEDDINGCONCRETE_H
#define FUZZYSTRINGMATCHING2_MULTIPLEPATHSEMBEDDINGCONCRETE_H

#include <string>
#include <embeddings/path_embedding/MultiplePathsEmbeddingStrategy.h>

struct MultiplePathsEmbeddingConcrete : public MultiplePathsEmbeddingStrategy {
    MultiplePathsEmbeddingConcrete(const std::string& varepsilon, bool doNotVisitLoopsTwice = true, size_t maxPathLength = std::numeric_limits<size_t>::max(),
                                   const double minimumPathCost = 2.0 * std::numeric_limits<double>::epsilon()) :
            MultiplePathsEmbeddingStrategy(varepsilon, doNotVisitLoopsTwice, maxPathLength, minimumPathCost) {}

    virtual ReadGraph::unstructured_embedding generatePathEmbedding(ReadGraph& rg, const path_info& path);;
};


#endif //FUZZYSTRINGMATCHING2_MULTIPLEPATHSEMBEDDINGCONCRETE_H
