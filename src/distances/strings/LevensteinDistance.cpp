//
// Created by giacomo on 20/10/20.
//

#include "distances/strings/LevensteinDistance.h"
#include <distances/strings/Levenstein.h>

LevensteinDistance::LevensteinDistance(const size_t insertCost, const size_t deleteCost, const size_t replaceCost)
        : insert_cost(insertCost), delete_cost(deleteCost), replace_cost(replaceCost) {}

double LevensteinDistance::distance(const std::string &left, const std::string &right) const {
    return GeneralizedLevensteinDistance(left, right, insert_cost, delete_cost, replace_cost);
}
