//
// Created by giacomo on 19/10/20.
//

#include "utils/AlterString.h"

AlterString::AlterString(const std::string &chrs, double noiseThreshold, size_t seedError) :
                         errorType{0.0,1.0}, pick(0, chrs.size()-2), chrs{chrs},
                         noiseThreshold{noiseThreshold}, seedError{seedError} {
    mersenneError.seed(seedError);
}

std::string AlterString::alter(std::string toAlter) {
    auto it = toAlter.begin();
    size_t originalSize = toAlter.size();
    size_t i = 0;
    while ((!toAlter.empty()) && (i< originalSize) &&  it != toAlter.end() ) {
        int casus = doNoise();
        if (toAlter.empty() && (casus != 1)) casus = 1;
        if (casus == 1) {
            // Add character
            char c = (char)chrs[pick(mersenneValue)];
            it = toAlter.insert(it, c);
            originalSize++;
        } if (casus == 2) {
            // Remove character
            it = toAlter.erase(it);
            originalSize--;
        } if (casus == 3) {
            // Replace character
            std::uniform_int_distribution<size_t> randomPosition{0UL, (size_t)std::distance(toAlter.begin(), it)};
            size_t j = randomPosition(mersennePosition);
            std::iter_swap(it, toAlter.begin()+j);
            it++;
        } else {
            it++;
        }
        i++;
    }
    return toAlter;
}
