//
// Created by giacomo on 21/10/20.
//

#include "points/PointTransformation.h"
#include <chrono>

std::unordered_map<TypesOfPointTransformation, std::vector<double>> PointTransformation::transformation;

PointClass PointTransformation::transform(const PointClass &pt, TypesOfPointTransformation t) {
    PointClass toRet = pt.asSimilarityVector();
    size_t benchmark_time;
    switch (t) {
        case STRAIGHTFORWARD_SQRT: {
            auto start = std::chrono::system_clock::now();
            double dist = std::sqrt(toRet.cost*toRet.cost + toRet.precision*toRet.precision);
            PointClass calc{1.0/(toRet.cost*dist)+1.0, 1.0/(toRet.precision*dist)+1.0, false};
            auto end = std::chrono::system_clock::now();
            benchmark_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            toRet = calc;
            break;
        }
        case BASIC_ROTATIONAL: {
            auto start = std::chrono::system_clock::now();
            double cost = toRet.cost * toRet.precision;
            double dist = std::sqrt(toRet.cost*toRet.cost + toRet.precision*toRet.precision);
            double frac = (cost - 1.0)/dist;
            PointClass calc{1.0+toRet.cost * frac, 1.0+toRet.precision * frac, false};
            auto end = std::chrono::system_clock::now();
            benchmark_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            toRet = calc;
            break;
        }
    }
    transformation[t].emplace_back(benchmark_time);
}
