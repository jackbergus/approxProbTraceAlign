/*
 * LevensteinPathSimilarity.h
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

#ifndef FUZZYSTRINGMATCHING2_LEVENSTEINPATHSIMILARITY_H
#define FUZZYSTRINGMATCHING2_LEVENSTEINPATHSIMILARITY_H


#include <matrix_graph_path/path_info.h>

struct LevensteinPathSimilarity  {
    double similarity(const path_info& left, const path_info& right) const;
};

#endif //FUZZYSTRINGMATCHING2_LEVENSTEINPATHSIMILARITY_H
