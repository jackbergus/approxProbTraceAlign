//
// Created by giacomo on 17/11/20.
//

#include "benchmarking/new/benchmark_common_ground.h"

benchmark_common_ground::benchmark_common_ground(const std::string &inputFile, const std::string &tracesFile,
                                                 const std::string &query, size_t queryLength,
                                                 PathEmbeddingStrategy embeddingStrategy, double traceNoise,
                                                 bool dulcior, double tuningFactor, bool usePathWithLambdaFactor,
                                                 double lambda, size_t maxLength, double minProb) : input_file(
        inputFile), traces_file(tracesFile), query(query), query_length(queryLength), embeddingStrategy(
        embeddingStrategy), trace_noise(traceNoise), dulcior(dulcior), tuning_factor(tuningFactor),
                                                                                                    use_path_with_lambda_factor(
                                                                                                            usePathWithLambdaFactor),
                                                                                                    lambda(lambda),
                                                                                                    max_length(
                                                                                                            maxLength),
                                                                                                    min_prob(minProb) {}

benchmark_common_ground::benchmark_common_ground() {}
