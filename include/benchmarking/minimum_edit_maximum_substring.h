/*
 * minimum_edit_maximum_substring.h
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
// Created by giacomo on 16/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_MINIMUM_EDIT_MAXIMUM_SUBSTRING_H
#define FUZZYSTRINGMATCHING2_MINIMUM_EDIT_MAXIMUM_SUBSTRING_H

#include <vector>

/**
 * Represents the proposed ranking ordered using the values of the expected ranking. By doing so, we are able to identify
 * the longest best subsequence
 * @param ranking   Expected order of the ranking
 * @return          A value between 0 and 1, where 1 identifies the fact that the returned ranking matches with the
 *                  expected one, and zero returns that there is no correlation between the two rankings
 */

double minimum_edit_maximum_substring(const std::vector<size_t>&  ranking);

#include <benchmarking/Ranking.h>

/**
 *  Represents the proposed ranking ordered using the values of the expected ranking. By doing so, we are able to identify
 * the longest best subsequence
 *
 * @tparam T            Key type of the ranking
 * @param competitor    object generating the ranking to be tested
 * @param expected      object generating the expected ranking
 * @return              Ranking result as in minimum_edit_maximum_substring
 */
template <typename T> double minimum_edit_maximum_substring(Ranking<T>& competitor, Ranking<T> expected) {
    return minimum_edit_maximum_substring(competitor.reorderForInducedRanking(expected));
}

#endif //FUZZYSTRINGMATCHING2_MINIMUM_EDIT_MAXIMUM_SUBSTRING_H
