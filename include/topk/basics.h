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

struct trivial_vector_record {
    std::pair<size_t, size_t> graph_and_trace_id;
    std::vector<double>       vector;

    trivial_vector_record();
    trivial_vector_record(const trivial_vector_record&) = default;
    trivial_vector_record& operator=(const trivial_vector_record&) = default;
    trivial_vector_record(const std::pair<size_t, size_t> &graphAndTraceId, const std::vector<double> &vector);
};

enum UnterstuetzenStrategie {
    ProbabilitySimilarity = 1,
    TransformedSpace = 2,
    EuclideanSpace = 4
};

#endif //FUZZYSTRINGMATCHING2_BASICS_H
