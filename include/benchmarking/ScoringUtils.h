/*
 * ScoringUtils.h
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
// Created by giacomo on 17/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_SCORINGUTILS_H
#define FUZZYSTRINGMATCHING2_SCORINGUTILS_H


#include <distances/strings/LevensteinSimilarity.h>
#include <embeddings/path_embedding/MultiplePathsEmbeddingStrategy.h>
#include <embeddings/graph_embedding/GraphEmbeddingStrategy.h>
#include <topk/Aussageform/DistanceExpressionEvaluator.h>
#include <magic_enum.hpp>
#include <set>
#include <embeddings/labelled_paths/NodesWithTransitiveEdgeCost.h>
#include <utils/AlterString.h>
#include <distances/strings/Levenstein.h>
#include <benchmarking/Ranking.h>
#include <benchmarking/minimum_edit_maximum_substring.h>

Ranking<size_t> doDulcior(Ranking<size_t> &orig, size_t N);

double score(const Eigen::VectorXd &left, const Eigen::VectorXd &right);

#endif //FUZZYSTRINGMATCHING2_SCORINGUTILS_H
