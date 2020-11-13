//
// Created by giacomo on 13/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_LOAD_PREGEX_H
#define FUZZYSTRINGMATCHING2_LOAD_PREGEX_H

#include <string>
#include <GenericGraph.h>

GenericGraph<size_t> load_pregex(const std::string& filename, const std::string& epsilon);

#endif //FUZZYSTRINGMATCHING2_LOAD_PREGEX_H
