//
// Created by giacomo on 21/10/20.
//

#include <distances/strings/Levenstein.h>
#include "distances/path/LevensteinPathSimilarity.h"

double LevensteinPathSimilarity::similarity(const path_info &left, const path_info &right) const {
    return GeneralizedLevensteinDistance(left.path, right.path);
}
