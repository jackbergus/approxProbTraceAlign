//
// Created by giacomo on 21/10/20.
//

#include <distances/strings/Levenstein.h>
#include "distances/strings/LevensteinSimilarity.h"

double LevensteinSimilarity::similarity(const std::string &left, const std::string &right) const {
    return 1.0 / (GeneralizedLevensteinDistance(left, right)/5.0+1.0);
}

LevensteinSimilarity::LevensteinSimilarity(size_t insertCost, size_t deleteCost, size_t replaceCost) : insert_cost{insertCost}, delete_cost{deleteCost}, replace_cost{replaceCost} {}
