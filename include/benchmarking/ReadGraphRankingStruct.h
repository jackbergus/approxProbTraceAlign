//
// Created by giacomo on 21/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_READGRAPHRANKINGSTRUCT_H
#define FUZZYSTRINGMATCHING2_READGRAPHRANKINGSTRUCT_H

#include <string>
#include <ReadGraph.h>

struct ReadGraphRankingStruct {
    std::string readGraphPath;
    ReadGraph*  graph;

    bool operator==(const ReadGraphRankingStruct &rhs) const;
    bool operator!=(const ReadGraphRankingStruct &rhs) const;
};

namespace std {
    template <>
    struct hash<ReadGraphRankingStruct>
    {
        std::size_t operator()(const ReadGraphRankingStruct& k) const
        {
            return hash_combine<std::string>(hash_combine<double>(31, (size_t)k.graph), k.readGraphPath);
        }
    };

}

#endif //FUZZYSTRINGMATCHING2_READGRAPHRANKINGSTRUCT_H
