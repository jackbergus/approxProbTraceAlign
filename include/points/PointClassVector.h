//
// Created by giacomo on 21/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_POINTCLASSVECTOR_H
#define FUZZYSTRINGMATCHING2_POINTCLASSVECTOR_H


#include "PointClass.h"
#include "PointTransformation.h"
#include <vector>
#include <benchmarking/Ranking.h>

class PointClassVector {
    std::vector<PointClass> set;
    Ranking<size_t> localRanking;

public:
    void addVector(double cost, double precision);

    PointClassVector() = default;
    PointClassVector(const PointClassVector&) = default;
    PointClassVector& operator=(const PointClassVector&) = default;

    PointClassVector transformToVector(TypesOfPointTransformation t) {

    }
};


#endif //FUZZYSTRINGMATCHING2_POINTCLASSVECTOR_H
