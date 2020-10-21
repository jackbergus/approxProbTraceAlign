//
// Created by giacomo on 19/10/20.
//

#include <matrix_graph_path/path_info.h>
#include <tuple>

path_info::path_info(double cost, const std::string &path, const std::vector<size_t>& v) : cost(cost), path(path), actualPath(v) {}

path_info::path_info() : cost{0.0}, path{""} {}

std::ostream &operator<<(std::ostream &os, const path_info &info) {
    os << "cost: " << info.cost << " path: " << info.path;
    return os;
}

bool path_info::operator==(const path_info &rhs) const {
    return std::tie(cost, path, actualPath) == std::tie(rhs.cost, rhs.path, rhs.actualPath);
}

bool path_info::operator!=(const path_info &rhs) const {
    return !(rhs == *this);
}
