//
// Created by giacomo on 21/10/20.
//

#include "points/PointClassVector.h"

void PointClassVector::addVector(double cost, double precision) {
    const PointClass& point = set.emplace_back(cost, precision, true);
    localRanking.addScore(set.size()-1, point.getSimilarityRanking());
}
