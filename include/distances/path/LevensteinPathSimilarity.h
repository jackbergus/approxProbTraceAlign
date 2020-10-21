//
// Created by giacomo on 21/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_LEVENSTEINPATHSIMILARITY_H
#define FUZZYSTRINGMATCHING2_LEVENSTEINPATHSIMILARITY_H


#include <matrix_graph_path/path_info.h>

struct LevensteinPathSimilarity  {
    double similarity(const path_info& left, const path_info& right) const;
};

#endif //FUZZYSTRINGMATCHING2_LEVENSTEINPATHSIMILARITY_H
