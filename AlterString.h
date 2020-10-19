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
    const std::string& chrs;
    double noiseThreshold;


    /**
     * Initialization
     * @param chrs                  Strings to insert
     * @param noiseThreshold        Threshold under which perform the changes
     * @param seedError             Seed for generating the error distribution (no change, insertion, deletion, swap)
     * @param seedValue             Seed for replacing the value to substitute
     * @param randomPosition        Seed for generating the random swap position
     */
    AlterString(const std::string& chrs, double noiseThreshold, size_t seedError = 0, size_t seedValue = 1, size_t randomPosition = 2);

    /**
     * Alters a string
     * @param toAlter   The string to alter, passed by copy
     * @return          The altered string
     */
    std::string alter(std::string toAlter);

private:
    int doNoise() {
        return errorType(mersenneError) <= noiseThreshold;
    }

};



#endif //FUZZYSTRINGMATCHING2_ALTERSTRING_H
