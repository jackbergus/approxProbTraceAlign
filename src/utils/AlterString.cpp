/*
 * AlterString.cpp
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

#include "utils/AlterString.h"

AlterString::AlterString(const std::string &chrs, double noiseThreshold, size_t seedError) :
                         errorType{0.0,1.0}, pick(0, chrs.size()-2), chrs{chrs},
                         noiseThreshold{noiseThreshold}, seedError{seedError} {
    mersenneError.seed(seedError);
}

std::string AlterString::alter(std::string toAlter) {
    auto it = toAlter.begin();
    size_t originalSize = toAlter.size();
    size_t i = 0;
    while ((!toAlter.empty()) && (i< originalSize) &&  it != toAlter.end() ) {
        int casus = doNoise();
        if (toAlter.empty() && (casus != 1)) casus = 1;
        if (casus == 1) {
            // Add character
            char c = (char)chrs[pick(mersenneValue)];
            it = toAlter.insert(it, c);
            originalSize++;
        } if (casus == 2) {
            // Remove character
            it = toAlter.erase(it);
            originalSize--;
        } if (casus == 3) {
            // Replace character
            std::uniform_int_distribution<size_t> randomPosition{0UL, (size_t)std::distance(toAlter.begin(), it)};
            size_t j = randomPosition(mersennePosition);
            std::iter_swap(it, toAlter.begin()+j);
            it++;
        } else {
            it++;
        }
        i++;
    }
    return toAlter;
}
