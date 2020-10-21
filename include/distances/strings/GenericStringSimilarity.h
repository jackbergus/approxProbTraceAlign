//
// Created by giacomo on 21/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_STRUCT_GENERICSTRINGSIMILARITY_H
#define FUZZYSTRINGMATCHING2_STRUCT_GENERICSTRINGSIMILARITY_H

#include <string>

struct GenericStringSimilarity {
        virtual double similarity(const std::string &left, const std::string &right) const = 0;
};

#endif //FUZZYSTRINGMATCHING2_STRUCT_GENERICSTRINGSIMILARITY_H
