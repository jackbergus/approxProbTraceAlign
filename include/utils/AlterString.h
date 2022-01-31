/*
 * AlterString.h
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
// Created by giacomo on 19/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_ALTERSTRING_H
#define FUZZYSTRINGMATCHING2_ALTERSTRING_H



#include <random>

/**
 * Alters a string provided as an input
 */
struct AlterString {
    std::uniform_real_distribution<double>  errorType;
    std::uniform_int_distribution<size_t>  valueType;
    std::mt19937 mersenneError, mersenneValue, mersennePosition;
    std::uniform_int_distribution<size_t> pick;
    std::string chrs;
    double noiseThreshold;
    size_t seedError = 0;

    AlterString() : AlterString("abcdefghijklmnopqrtsuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 0.3) {}
    AlterString(const AlterString& ) = default;
    AlterString& operator=(const AlterString& ) = default;

    /**
     * Initialization
     * @param chrs                  Strings to insert
     * @param noiseThreshold        Threshold under which perform the changes
     * @param seedError             Seed for generating the error distribution (no change, insertion, deletion, swap)
     */
    AlterString(const std::string& chrs, double noiseThreshold, size_t seedError = 0);

    /**
     * Alters a string
     * @param toAlter   The string to alter, passed by copy
     * @return          The altered string
     */
    std::string alter(std::string toAlter);

    std::string alter(std::string toAlter, size_t maxRemoves, size_t maxInsertions) {
        auto it = toAlter.begin();
        size_t originalSize = toAlter.size();
        size_t i = 0;

        while (((maxRemoves != 0) || (maxInsertions != 0))) {
            int casus = doNoise();
            if (toAlter.empty() && (casus != 1)) casus = 1;
            if (casus == 1) {
                // Add character
                if (maxInsertions > 0 ) {
                    char c = (char)chrs[pick(mersenneValue)];
                    it = toAlter.insert(it, c);
                    originalSize++;
                    maxInsertions--;
                }
            } if (casus == 2) {
                // Remove character
                if (maxRemoves > 0) {
                    it = toAlter.erase(it);
                    originalSize--;
                    maxRemoves--;
                }
            } if (casus == 3) {
                // Replace character
                if ((maxRemoves> 0 ) && (maxInsertions> 0)) {
                    std::uniform_int_distribution<size_t> randomPosition{0UL, (size_t)std::distance(toAlter.begin(), it)};
                    size_t j = randomPosition(mersennePosition);
                    std::iter_swap(it, toAlter.begin()+j);
                    it++;
                    maxRemoves--;
                    maxInsertions--;
                }
            } else {
                it++;
            }
            if ((it == toAlter.end()) && ((maxRemoves != 0) || (maxInsertions != 0))) {
                it = toAlter.begin();
                originalSize = toAlter.size();
            }
        }
        return toAlter;
    }

private:
    int doNoise() {
        double randomNumber =  errorType(mersenneError);
        return randomNumber <= noiseThreshold ? std::min(3, (int) std::trunc(randomNumber / (noiseThreshold) * 3.0) + 1)
                                              : 0;
    }

};



#endif //FUZZYSTRINGMATCHING2_ALTERSTRING_H
