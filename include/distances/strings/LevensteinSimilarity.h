//
// Created by giacomo on 21/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_LEVENSTEINSIMILARITY_H
#define FUZZYSTRINGMATCHING2_LEVENSTEINSIMILARITY_H

#include <distances/strings/GenericStringSimilarity.h>
#include <string>

struct LevensteinSimilarity : public GenericStringSimilarity {
    const size_t insert_cost = 1;
    const size_t delete_cost = 1;
    const size_t replace_cost = 1;

    LevensteinSimilarity(const size_t insertCost = 1, const size_t deleteCost = 1, const size_t replaceCost = 1);
    double similarity(const std::string &left, const std::string &right) const override;
};


#endif //FUZZYSTRINGMATCHING2_LEVENSTEINSIMILARITY_H
