//
// Created by giacomo on 13/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_BASICS_H
#define FUZZYSTRINGMATCHING2_BASICS_H

#include <set>
#include <vector>
#include <matrix_graph_path/path_info.h>
#include <ReadGraph.h>

using TopKBenchmarkDataset = std::vector<std::pair<ReadGraph, std::vector<struct path_info>>>;
using EinbettungRaum = std::set<std::pair<std::string,std::string>>;

enum UnterstuetzenStrategie {
    ProbabilitySimilarity = 1,
    TransformedSpace = 2,
    EuclideanSpace = 4
};

#endif //FUZZYSTRINGMATCHING2_BASICS_H
