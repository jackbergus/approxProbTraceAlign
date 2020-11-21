/*
 * LevensteinDistance.cpp
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

#include "distances/strings/LevensteinDistance.h"
#include <distances/strings/Levenstein.h>

LevensteinDistance::LevensteinDistance(const size_t insertCost, const size_t deleteCost, const size_t replaceCost)
        : insert_cost(insertCost), delete_cost(deleteCost), replace_cost(replaceCost) {}

double LevensteinDistance::distance(const std::string &left, const std::string &right) const {
    return GeneralizedLevensteinDistance(left, right, insert_cost, delete_cost, replace_cost);
}
