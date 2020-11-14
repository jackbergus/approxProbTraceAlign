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
