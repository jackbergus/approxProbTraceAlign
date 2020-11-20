//
// Created by giacomo on 13/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_LOAD_XES_H
#define FUZZYSTRINGMATCHING2_LOAD_XES_H

#include <vector>
#include <string>
#include <data_loading/local_load_data.h>
#include <fptree.hpp>

std::vector<Transaction<TimestampedEvent>> load_xes(const std::string& filename = "/home/giacomo/Immagini/split-miner-2.0/sepsis.xes");

#endif //FUZZYSTRINGMATCHING2_LOAD_XES_H
