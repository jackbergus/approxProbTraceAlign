//
// Created by giacomo on 13/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_LOAD_PETRI_MATRIX_H
#define FUZZYSTRINGMATCHING2_LOAD_PETRI_MATRIX_H


#include <GenericGraph.h>

GenericGraph<size_t> load_petri_matrix(const std::string& filename = "data/petri_matrix.txt", bool isFile = true);

#endif //FUZZYSTRINGMATCHING2_LOAD_PETRI_MATRIX_H
