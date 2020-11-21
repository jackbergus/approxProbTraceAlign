/*
 * LevensteinSimilarity.cpp
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

#include <distances/strings/Levenstein.h>
#include "distances/strings/LevensteinSimilarity.h"

double LevensteinSimilarity::similarity(const std::string &left, const std::string &right) const {
    return 1.0 / (GeneralizedLevensteinDistance(left, right)/5.0+1.0);
}

LevensteinSimilarity::LevensteinSimilarity(size_t insertCost, size_t deleteCost, size_t replaceCost) : insert_cost{insertCost}, delete_cost{deleteCost}, replace_cost{replaceCost} {}
