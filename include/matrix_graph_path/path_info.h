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
    double      cost;
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

            std::size_t seed = k.actualPath.size();
            for(auto& i : k.actualPath) {
                seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return hash_combine<std::string>(hash_combine<double>(seed, k.cost), k.path);
        }
    };

}

#endif //FUZZYSTRINGMATCHING2_PATH_INFO_H
