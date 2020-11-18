//
// Created by giacomo on 17/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_MULTITHREADEDBENCHMARKFORPOOLING_H
#define FUZZYSTRINGMATCHING2_MULTITHREADEDBENCHMARKFORPOOLING_H


#include <benchmarking/new/record_element_per_query.h>
#include <benchmarking/new/additional_benchmarks_per_log.h>
#include <distances/strings/LevensteinSimilarity.h>
#include <embeddings/path_embedding/MultiplePathsEmbeddingStrategy.h>
#include <embeddings/graph_embedding/GraphEmbeddingStrategy.h>
#include <topk/Aussageform/DistanceExpressionEvaluator.h>
#include <magic_enum.hpp>
#include <set>
#include <embeddings/labelled_paths/NodesWithTransitiveEdgeCost.h>
#include <utils/AlterString.h>
#include <distances/strings/Levenstein.h>
#include <benchmarking/Ranking.h>
#include <topk/old/minimum_edit_maximum_substring.h>

Ranking<size_t> doDulcior(Ranking<size_t> &orig, size_t N);

double score(const Eigen::VectorXd &left, const Eigen::VectorXd &right);

struct MultithreadedBenchmarkForPooling {
public:
//output
    std::vector<record_element_per_query> output_quality;
    std::vector<additional_benchmarks_per_log> log_quality;

private:
    //input
    size_t thread_id_for_logging;
    PathEmbeddingStrategy casus;
    std::string epsilon;
    benchmark_common_ground param;
    DistanceExpressionEvaluator *probSimMetric;
    ReadGraph copyGraph;
    std::string charList;
    size_t seed;
    std::vector<double> &noiseThreshold;
    std::vector<struct path_info> &finalLog;
    std::vector<struct path_info> &finalOriginalLog;

// not requiring init
    LevensteinSimilarity similarity;
    MultiplePathsEmbeddingStrategy *pathstrategy = nullptr; //after parameters init
    GraphEmbeddingStrategy *graphStrategy = nullptr; // after parameters init

    MultiplePathsEmbeddingStrategy *generatePathEmbeddingStrategyFromParameters() const;

    GraphEmbeddingStrategy *generateGraphEmbeddingStrategyFromParameters() const;

    double set_of_path_similarity(const std::vector<path_info> &tracesSet, const std::vector<path_info> &querySet,
                                  const LevensteinSimilarity &sim,
                                  std::vector<additional_benchmarks_per_log> *opt = nullptr,
                                  double *precisionNormalization = nullptr, PathEmbeddingStrategy *strategy = nullptr);

public:

    MultithreadedBenchmarkForPooling(size_t threadIdForLogging, PathEmbeddingStrategy casus, const std::string &epsilon,
                                     const benchmark_common_ground &param, DistanceExpressionEvaluator *probSimMetric,
                                     const ReadGraph &copyGraph, const std::string &charList, size_t seed,
                                     std::vector<double> &noiseThreshold, std::vector<struct path_info> &finalLog,
                                     std::vector<struct path_info> &finalOriginalLog);

    std::pair<std::vector<record_element_per_query>, std::vector<additional_benchmarks_per_log>> run();
    void performBenchmark2(std::set<std::pair<std::string, std::string>> &embedding_space,
                           const std::unordered_map<struct path_info, Eigen::VectorXd> &map,
                           std::string &actualQuery, double noise,
                           std::vector<struct path_info> &pathsOrder);


};


#endif //FUZZYSTRINGMATCHING2_MULTITHREADEDBENCHMARKFORPOOLING_H
