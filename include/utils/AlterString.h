//
// Created by giacomo on 19/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_ALTERSTRING_H
#define FUZZYSTRINGMATCHING2_ALTERSTRING_H



#include <random>

/**
 * Alters a string provided as an input
 */
struct AlterString {
    std::uniform_real_distribution<double>  errorType;
    std::uniform_int_distribution<size_t>  valueType;
    std::mt19937 mersenneError, mersenneValue, mersennePosition;
    std::uniform_int_distribution<size_t> pick;
    std::string chrs;
    double noiseThreshold;
    size_t seedError = 0;

    AlterString() : AlterString("abcdefghijklmnopqrtsuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 0.3) {}
    AlterString(const AlterString& ) = default;
    AlterString& operator=(const AlterString& ) = default;

    /**
     * Initialization
     * @param chrs                  Strings to insert
     * @param noiseThreshold        Threshold under which perform the changes
     * @param seedError             Seed for generating the error distribution (no change, insertion, deletion, swap)
     */
    AlterString(const std::string& chrs, double noiseThreshold, size_t seedError = 0);

    /**
     * Alters a string
     * @param toAlter   The string to alter, passed by copy
     * @return          The altered string
     */
    std::string alter(std::string toAlter);

private:
    int doNoise() {
        double randomNumber =  errorType(mersenneError);
        return randomNumber <= noiseThreshold ? std::min(3, (int) std::trunc(randomNumber / (noiseThreshold) * 3.0) + 1)
                                              : 0;
    }

};



#endif //FUZZYSTRINGMATCHING2_ALTERSTRING_H
