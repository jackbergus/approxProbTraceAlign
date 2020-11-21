/*
 * LevensteinDistance.h
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

#ifndef FUZZYSTRINGMATCHING2_LEVENSTEINDISTANCE_H
#define FUZZYSTRINGMATCHING2_LEVENSTEINDISTANCE_H

#include <string>
#include "GenericStringDistance.h"

class LevensteinDistance : public GenericStringDistance {
    const size_t insert_cost = 1;
    const size_t delete_cost = 1;
    const size_t replace_cost = 1;

public:
    LevensteinDistance(const size_t insertCost = 1, const size_t deleteCost = 1, const size_t replaceCost = 1);
    double distance(const std::string &left, const std::string &right) const override;

};

#endif //FUZZYSTRINGMATCHING2_LEVENSTEINDISTANCE_H
