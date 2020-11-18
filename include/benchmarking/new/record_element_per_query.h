//
// Created by giacomo on 17/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_RECORD_ELEMENT_PER_QUERY_H
#define FUZZYSTRINGMATCHING2_RECORD_ELEMENT_PER_QUERY_H


#include <benchmarking/new/benchmark_common_ground.h>

struct record_element_per_query : public benchmark_common_ground {
    double rankingDistance;
    double spearman;
    double proposedMetric;

    record_element_per_query() : benchmark_common_ground() {}
    record_element_per_query(const record_element_per_query&) = default;
    record_element_per_query& operator=(const record_element_per_query&) = default;
    record_element_per_query(const std::string &inputFile, const std::string &tracesFile,
                             const std::string &query, size_t queryLength,
                             PathEmbeddingStrategy embeddingStrategy, double traceNoise,

                             double rankingDistance, double spearman, double proposedMetric,

                             bool dulcior, double tuningFactor, bool usePathWithLambdaFactor, double lambda, size_t maxLength, double minProb);
};



#endif //FUZZYSTRINGMATCHING2_RECORD_ELEMENT_PER_QUERY_H
