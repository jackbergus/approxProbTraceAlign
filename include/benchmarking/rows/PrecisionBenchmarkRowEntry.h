//
// Created by giacomo on 20/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_PRECISIONBENCHMARKROWENTRY_H
#define FUZZYSTRINGMATCHING2_PRECISIONBENCHMARKROWENTRY_H

enum case_precision_type {
    SCORE_EXPECTED_FROM_QUERY_SET,
    RANK_EXPECTED_FROM_QUERY_SET,
    RANK_PRECISION_EXPECTED_FROM_QUERY_SET,
    SCORE_EXPECTED_FROM_EXPECTED_SET,
    RANK_EXPECTED_FROM_EXPECTED_SET,
    RANK_PRECISION_EXPECTED_FROM_EXPECTED_SET,
    SET_DISTANCE,
    SET_DIFFERENCE
};

#include <string>
#include <fort.hpp>

struct PrecisionBenchmarkRowEntry {
    std::string dataset_name, configuration;
    size_t query_size, distinct_query_chars;
    case_precision_type eSCase;
    double expectedScore;

    PrecisionBenchmarkRowEntry(const std::string &datasetName, const std::string &configuration, size_t querySize,
                               size_t distinctQueryChars, case_precision_type eSCase, double expectedScore);

    friend std::ostream &operator<<(std::ostream &os, const PrecisionBenchmarkRowEntry &entry);
    friend fort::char_table &operator<<(fort::char_table &os, const PrecisionBenchmarkRowEntry &entry);
    static fort::char_table & init_header(fort::char_table &os);
};


#endif //FUZZYSTRINGMATCHING2_PRECISIONBENCHMARKROWENTRY_H
