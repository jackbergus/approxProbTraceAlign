//
// Created by giacomo on 14/11/20.
//

#include <topk/basics.h>

trivial_vector_record::trivial_vector_record() {}

trivial_vector_record::trivial_vector_record(const std::pair<size_t, size_t> &graphAndTraceId,
                                             const std::vector<double> &vector)
        : graph_and_trace_id(graphAndTraceId), vector(vector) {}
