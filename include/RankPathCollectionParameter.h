/*
 * RankPathCollectionParameter.h
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
// Created by giacomo on 21/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_RANKPATHCOLLECTIONPARAMETER_H
#define FUZZYSTRINGMATCHING2_RANKPATHCOLLECTIONPARAMETER_H

#if 0
#include <string>
#include <benchmarking/Ranking.h>
#include "ReadGraph.h"

template <typename Element, typename RankingArgument> struct RankPathCollectionParameter {
    virtual std::pair<size_t, size_t> runQuery(const Element &el, const std::string &query, Ranking<RankingArgument>& rank, double& benchmark_time, ReadGraph *rg, BenchmarkStrategy* strategy) = 0;
};

#endif //FUZZYSTRINGMATCHING2_RANKPATHCOLLECTIONPARAMETER_H
#endif
