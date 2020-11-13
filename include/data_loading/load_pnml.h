//
// Created by giacomo on 13/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_LOAD_PNML_H
#define FUZZYSTRINGMATCHING2_LOAD_PNML_H

#include <vector>
#include <string>
#include <GenericGraph.h>

/**
 * Parsing the files generate by the toolspecific StochasticPetriNet
 */
std::vector<GenericGraph<size_t>> load_pnml(const std::string& filename = "/home/giacomo/Immagini/split-miner-2.0/petri_net_30.pnml", const std::string& epsilon = ".");

#endif //FUZZYSTRINGMATCHING2_LOAD_PNML_H
