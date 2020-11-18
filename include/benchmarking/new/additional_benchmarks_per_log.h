//
// Created by giacomo on 17/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_ADDITIONAL_BENCHMARKS_PER_LOG_H
#define FUZZYSTRINGMATCHING2_ADDITIONAL_BENCHMARKS_PER_LOG_H


#include <benchmarking/new/benchmark_common_ground.h>

enum LogDependantQualityMetricTypes {
    LogGeneralizationMetric,
    LogPrecisionMetric
};

struct additional_benchmarks_per_log : public benchmark_common_ground {
    LogDependantQualityMetricTypes metricType;
    double                         value;
    std::string                    logtrace;
    size_t                         logtrace_size;

    additional_benchmarks_per_log() : benchmark_common_ground() {}
    additional_benchmarks_per_log(const additional_benchmarks_per_log&) = default;
    additional_benchmarks_per_log& operator=(const additional_benchmarks_per_log&) = default;
    additional_benchmarks_per_log(const std::string &inputFile,
                                    const std::string &tracesFile,
                                    const std::string &query,
                                    size_t queryLength,
                                    PathEmbeddingStrategy embeddingStrategy,
                                    double traceNoise,
                                    bool dulcior,
                                    double tuningFactor,
                                    bool usePathWithLambdaFactor,
                                    double lambda,
                                    size_t maxLength,
                                    double minProb,
                                    LogDependantQualityMetricTypes metricType,
                                    double value,
                                    const std::string &logtrace,
                                    size_t logtraceSize);
};

#endif //FUZZYSTRINGMATCHING2_ADDITIONAL_BENCHMARKS_PER_LOG_H
