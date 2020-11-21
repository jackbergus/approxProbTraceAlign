/*
 * path_info.h
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

#ifndef FUZZYSTRINGMATCHING2_PATH_INFO_H
#define FUZZYSTRINGMATCHING2_PATH_INFO_H

#include <string>
#include <ostream>
#include <vector>
#include <utils/hash_combine.h>

struct path_info {
    double      probability;
    std::string path;
    std::vector<size_t> actualPath;

    path_info(double cost, const std::string &path, const std::vector<size_t>& ap);
    path_info();
    path_info(const path_info&) = default;
    path_info& operator=(const path_info&) = default;

    friend std::ostream &operator<<(std::ostream &os, const path_info &info);

    bool operator==(const path_info &rhs) const;

    bool operator!=(const path_info &rhs) const;
};

namespace std {
    template <>
    struct hash<struct path_info>
    {
        std::size_t operator()(const struct path_info& k) const
        {

            // The hashing is defined for the hashmap of Ranking, where the only relevant fact is the path itself
            std::size_t seed = k.path.size();
            /*for(auto& i : k.actualPath) {
                seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }*/
            return hash_combine<std::string>(/*hash_combine<double>(seed, k.path)*/ seed, k.path);
        }
    };

}

#endif //FUZZYSTRINGMATCHING2_PATH_INFO_H
