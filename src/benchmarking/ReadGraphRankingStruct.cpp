//
// Created by giacomo on 21/10/20.
//

#include "benchmarking/ReadGraphRankingStruct.h"

bool ReadGraphRankingStruct::operator==(const ReadGraphRankingStruct &rhs) const {
    return readGraphPath == rhs.readGraphPath &&
           graph == rhs.graph;
}

bool ReadGraphRankingStruct::operator!=(const ReadGraphRankingStruct &rhs) const {
    return !(rhs == *this);
}
