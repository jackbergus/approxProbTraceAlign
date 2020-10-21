//
// Created by giacomo on 20/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_LEVENSTEINDISTANCE_H
#define FUZZYSTRINGMATCHING2_LEVENSTEINDISTANCE_H

#include <string>
#include "GenericStringDistance.h"

class LevensteinDistance : public GenericStringDistance {
    const size_t insert_cost = 1;
    const size_t delete_cost = 1;
    const size_t replace_cost = 1;

public:
    LevensteinDistance(const size_t insertCost = 1, const size_t deleteCost = 1, const size_t replaceCost = 1);
    double distance(const std::string &left, const std::string &right) const override;

};

#endif //FUZZYSTRINGMATCHING2_LEVENSTEINDISTANCE_H
