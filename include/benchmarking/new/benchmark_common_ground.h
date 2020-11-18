//
// Created by giacomo on 17/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_BENCHMARK_COMMON_GROUND_H
#define FUZZYSTRINGMATCHING2_BENCHMARK_COMMON_GROUND_H

#include <embeddings/labelled_paths/LabelledPathVisitingStrategy.h>

struct benchmark_common_ground {
    std::string input_file;
    std::string traces_file;
    std::string query;
    size_t query_length;
    PathEmbeddingStrategy embeddingStrategy;
    double trace_noise;

    bool dulcior;
    double tuning_factor;
    bool   use_path_with_lambda_factor;
    double lambda;
    size_t max_length;
    double min_prob;

    benchmark_common_ground();
    benchmark_common_ground(const benchmark_common_ground&) = default;
    benchmark_common_ground& operator=(const benchmark_common_ground&) = default;
    benchmark_common_ground(const std::string &inputFile, const std::string &tracesFile, const std::string &query,
                            size_t queryLength, PathEmbeddingStrategy embeddingStrategy, double traceNoise,
                            bool dulcior, double tuningFactor, bool usePathWithLambdaFactor, double lambda,
                            size_t maxLength, double minProb);
};


#endif //FUZZYSTRINGMATCHING2_BENCHMARK_COMMON_GROUND_H
