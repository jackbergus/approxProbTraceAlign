//
// Created by giacomo on 21/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_POINTTRANSFORMATION_H
#define FUZZYSTRINGMATCHING2_POINTTRANSFORMATION_H

enum TypesOfPointTransformation {
    BASIC_ROTATIONAL,
    STRAIGHTFORWARD_SQRT
};

#include <unordered_map>
#include <vector>
#include <cmath>
#include "PointClass.h"

class PointTransformation {

    static std::unordered_map<TypesOfPointTransformation, std::vector<double>> transformation;

    static PointClass transform(const PointClass& pt, TypesOfPointTransformation t);


};


#endif //FUZZYSTRINGMATCHING2_POINTTRANSFORMATION_H
