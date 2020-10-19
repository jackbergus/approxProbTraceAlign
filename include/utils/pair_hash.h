//
// Created by giacomo on 19/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_PAIR_HASH_H
#define FUZZYSTRINGMATCHING2_PAIR_HASH_H

#include <utility>
#include <functional>

struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pair) const
    {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

#endif //FUZZYSTRINGMATCHING2_PAIR_HASH_H
