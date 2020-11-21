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
