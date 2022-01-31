//
// Created by giacomo on 28/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_PNETUTILS_H
#define FUZZYSTRINGMATCHING2_PNETUTILS_H

#include <vector>
#include <unordered_map>
#include <ostream>
using Marking = std::vector<size_t>;
using Transition_to_Marking = std::unordered_map<size_t, Marking>;
std::ostream& operator<< (std::ostream& out, const  std::vector<size_t> & v);

#endif //FUZZYSTRINGMATCHING2_PNETUTILS_H
