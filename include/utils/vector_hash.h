/*
 * vector_hash.h
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
// Created by giacomo on 14/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_VECTOR_HASH_H
#define FUZZYSTRINGMATCHING2_VECTOR_HASH_H

#include <utils/hash_combine.h>

namespace std {
    template <typename T>
    struct hash<std::vector<T>>
    {
        std::size_t operator()(const std::vector<T>& k) const
        {
            size_t init = 31;
            for (const auto& x : k) init = hash_combine<T>(init, x);
            return init;
        }
    };

}

template <typename T>
struct vector_hash {
    std::size_t operator()(const std::vector<T>& k) const
    {
        size_t init = 31;
        for (const auto& x : k) init = hash_combine<T>(init, x);
        return init;
    }
};

#endif //FUZZYSTRINGMATCHING2_VECTOR_HASH_H
