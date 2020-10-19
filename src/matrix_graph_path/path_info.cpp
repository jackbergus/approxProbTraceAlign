//
// Created by giacomo on 19/10/20.
//

#include <matrix_graph_path/path_info.h>

path_info::path_info(double cost, const std::string &path, const std::vector<size_t>& v) : cost(cost), path(path), actualPath(v) {}

path_info::path_info() : cost{0.0}, path{""} {}

std::ostream &operator<<(std::ostream &os, const path_info &info) {
    os << "cost: " << info.cost << " path: " << info.path;
    return os;
}
