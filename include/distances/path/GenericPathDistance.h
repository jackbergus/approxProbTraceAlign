//
// Created by giacomo on 20/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_GENERICSTRINGPATHDISTANCE_H
#define FUZZYSTRINGMATCHING2_GENERICSTRINGPATHDISTANCE_H

#include <string>

struct GenericPathDistance {
    virtual double distance(const std::string& left, const struct path_info& right) = 0;
};

#endif //FUZZYSTRINGMATCHING2_GENERICSTRINGPATHDISTANCE_H
