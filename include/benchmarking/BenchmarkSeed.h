//
// Created by giacomo on 20/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_BENCHMARKSEED_H
#define FUZZYSTRINGMATCHING2_BENCHMARKSEED_H

#include <string>
#include <matrix_graph_path/path_info.h>

struct BenchmarkSeed {
    std::string         query;
    struct path_info    expected_result;

    BenchmarkSeed() = default;

    BenchmarkSeed(const std::string &query, const path_info &expectedResult) : query(query),
                                                                               expected_result(expectedResult) {}

    BenchmarkSeed(const BenchmarkSeed&) = default;
    BenchmarkSeed& operator=(const BenchmarkSeed&) = default;
};

#endif //FUZZYSTRINGMATCHING2_BENCHMARKSEED_H
