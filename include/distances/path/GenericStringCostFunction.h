/*
 * GenericStringCostFunction.h
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

#ifndef FUZZYSTRINGMATCHING2_GENERICSTRINGCOSTFUNCTION_H
#define FUZZYSTRINGMATCHING2_GENERICSTRINGCOSTFUNCTION_H

#include <distances/path/GenericPathDistance.h>
#include <distances/strings/GenericStringDistance.h>
#include <memory>

template <typename T>
        class GenericStringCostFunction : public GenericPathDistance {
    std::unique_ptr<GenericStringDistance> string_distance;
public:
    template <typename ...Args> GenericStringCostFunction(Args && ...args) : string_distance{new T(std::forward<Args>(args)...)}{

    }
    double distance(const std::string &left, const struct path_info &right) override {
            const double cost = string_distance->distance(left, right.path);
            return ((1.0/(cost/5.0+1.0))) * right.probability;
    }

};


#endif //FUZZYSTRINGMATCHING2_GENERICSTRINGCOSTFUNCTION_H
