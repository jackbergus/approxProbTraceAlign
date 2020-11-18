//
// Created by giacomo on 17/11/20.
//

#include "benchmarking/new/record_element_per_query.h"

record_element_per_query::record_element_per_query(const std::string &inputFile, const std::string &tracesFile,
                                                   const std::string &query, size_t queryLength,
                                                   PathEmbeddingStrategy embeddingStrategy, double traceNoise,
                                                   double rankingDistance,
                                                   double spearman, double proposedMetric, bool dulcior,
                                                   double tuningFactor,
                                                   bool usePathWithLambdaFactor, double lambda, size_t maxLength,
                                                   double minProb)
        : benchmark_common_ground(inputFile, tracesFile, query, queryLength, embeddingStrategy, traceNoise, dulcior,
                                  tuningFactor, usePathWithLambdaFactor, lambda, maxLength, minProb),

          rankingDistance(rankingDistance),
          spearman(spearman), proposedMetric(proposedMetric) {}
