//
// Created by giacomo on 19/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_PATH_INFO_H
#define FUZZYSTRINGMATCHING2_PATH_INFO_H

#include <string>
#include <ostream>

struct path_info {
    double      cost;
    std::string path;

    path_info(double cost, const std::string &path);
    path_info();
    path_info(const path_info&) = default;
    path_info& operator=(const path_info&) = default;

    friend std::ostream &operator<<(std::ostream &os, const path_info &info);
};

#endif //FUZZYSTRINGMATCHING2_PATH_INFO_H
