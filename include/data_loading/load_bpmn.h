//
// Created by giacomo on 13/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_LOAD_BPMN_H
#define FUZZYSTRINGMATCHING2_LOAD_BPMN_H

#include <vector>
#include <string>
#include <GenericGraph.h>


/**
 * Ingests the output of SplitMiner 2
 * @return
 */
std::vector<GenericGraph<size_t>> load_bpmn(const std::string& filename = "file:///home/giacomo/Immagini/split-miner-2.0/sepsis_out.xml", const std::string& epsilon = ".");


#endif //FUZZYSTRINGMATCHING2_LOAD_BPMN_H
