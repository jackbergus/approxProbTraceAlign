/*
 * path_info.cpp
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

#include <matrix_graph_path/path_info.h>
#include <tuple>

path_info::path_info(double probability, const std::string &path, const std::vector<size_t>& v) : probability(probability), path(path), actualPath(v) {}

path_info::path_info() : probability{0.0}, path{""} {}

std::ostream &operator<<(std::ostream &os, const path_info &info) {
    os << "cost: " << info.probability << " path: " << info.path;
    return os;
}

bool path_info::operator==(const path_info &rhs) const {
    // The comparison is used for the Rank function purposes, where we need to find in the map the paths that have the same path structure
    return path == rhs.path && probability == rhs.probability;//std::tie(cost, path, actualPath) == std::tie(rhs.cost, rhs.path, rhs.actualPath);
}

bool path_info::operator!=(const path_info &rhs) const {
    return !(rhs == *this);
}
