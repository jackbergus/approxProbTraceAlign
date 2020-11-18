//
// Created by giacomo on 17/11/20.
//

#include "benchmarking/new/additional_benchmarks_per_log.h"
additional_benchmarks_per_log::additional_benchmarks_per_log(const std::string &inputFile,
                                                             const std::string &tracesFile, const std::string &query,
                                                             size_t queryLength,
                                                             PathEmbeddingStrategy embeddingStrategy, double traceNoise,
                                                             bool dulcior, double tuningFactor,
                                                             bool usePathWithLambdaFactor, double lambda,
                                                             size_t maxLength, double minProb,
                                                             LogDependantQualityMetricTypes metricType, double value,
                                                             const std::string &logtrace, size_t logtraceSize)
        : benchmark_common_ground(inputFile, tracesFile, query, queryLength, embeddingStrategy, traceNoise, dulcior,
                                  tuningFactor, usePathWithLambdaFactor, lambda, maxLength, minProb),
          metricType(metricType), value(value), logtrace(logtrace), logtrace_size(logtraceSize) {}