//
// Created by giacomo on 20/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_GENERICSTRINGDISTANCE_H
#define FUZZYSTRINGMATCHING2_GENERICSTRINGDISTANCE_H

struct GenericStringDistance {
    virtual double distance(const std::string &left, const std::string &right) const = 0;
};

#endif //FUZZYSTRINGMATCHING2_GENERICSTRINGDISTANCE_H
