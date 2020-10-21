//
// Created by giacomo on 20/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_GENERICSTRINGCOSTFUNCTION_H
#define FUZZYSTRINGMATCHING2_GENERICSTRINGCOSTFUNCTION_H

#include <distances/path/GenericPathDistance.h>
#include <distances/strings/GenericStringDistance.h>
#include <memory>

template <typename T>
        class GenericStringCostFunction : public GenericPathDistance {
    std::unique_ptr<GenericStringDistance> string_distance;
public:
    template <typename ...Args> GenericStringCostFunction(Args && ...args) : string_distance{new T(std::forward<Args>(args)...)}{

    }
    double distance(const std::string &left, const struct path_info &right) override {
            const double cost = string_distance->distance(left, right.path);
            return ((1.0/(cost/5.0+1.0))) * right.cost;
    }

};


#endif //FUZZYSTRINGMATCHING2_GENERICSTRINGCOSTFUNCTION_H
