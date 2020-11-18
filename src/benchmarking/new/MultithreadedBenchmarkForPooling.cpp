//
// Created by giacomo on 17/11/20.
//

#include "benchmarking/new/MultithreadedBenchmarkForPooling.h"

Ranking<size_t> doDulcior(Ranking<size_t> &orig, size_t N) {
    Ranking<size_t> finalRanking;
    for (size_t i = 0; i<N; i++) {
        finalRanking.addScore(i, orig.getRanking(i));
    }
    return finalRanking;
}

double score(const Eigen::VectorXd& left, const Eigen::VectorXd& right) {
    return left.dot(right);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MultithreadedBenchmarkForPooling::performBenchmark2(std::set<std::pair<std::string, std::string>> &embedding_space,
                                                         const std::unordered_map<struct path_info, Eigen::VectorXd> &map,
                                                         std::string &actualQuery, double noise,
                                                         std::vector<struct path_info> &pathsOrder) {
    ReadGraph g = ReadGraph::fromString(actualQuery, 1.0);
    auto tmp = (*graphStrategy)(g);
    auto x = ReadGraph::generateStructuredEmbedding(embedding_space, tmp);
    Ranking<size_t> pathRanking, expectedRanking;
    ///std::map<double, std::vector<size_t>> rankingMap;
    size_t j = 0;
    double forGeneralization = -1.0;
    size_t genIdx = 0;
    for (const auto &paths : map) {
        double sc = score(x, paths.second);
        pathRanking.addScore(j, sc);
        double similarity = 1.0 / (GeneralizedLevensteinDistance(paths.first.path, actualQuery) / 5.0 + 1.0);
        if (similarity > forGeneralization) {
            genIdx = j;
            forGeneralization = similarity;
        }

        double finalScore = 1.0 - probSimMetric->operator()({paths.first.probability, similarity});
        expectedRanking.addScore(j, finalScore);
        ///rankingMap[finalScore].emplace_back(j);

        j++;
    }

    log_quality.emplace_back(param.input_file,
                             param.traces_file,
                             actualQuery,
                             actualQuery.size(),
                             casus,
                             noise,
                             false,
                             param.tuning_factor,
                             param.use_path_with_lambda_factor,
                             param.lambda,
                             param.max_length,
                             param.min_prob,
                             LogDependantQualityMetricTypes::LogGeneralizationMetric,
                             forGeneralization, "", 0);

    ///for (const auto& x : rankingMap) assert(x.second.size() == 1); // Imposing that there are no elements with the same scoring value
    const auto ls = [this, &pathsOrder](const size_t &left, const size_t &right) {
        return similarity.similarity(pathsOrder.at(left).path, pathsOrder.at(right).path);
    };
    double rankingDistance = expectedRanking.normalizedRank(pathRanking, ls);
    double spearman = expectedRanking.SpearmanCorrelationIndex(pathRanking, 1.0);
    double proposedMetric = minimum_edit_maximum_substring(pathRanking, expectedRanking);

    output_quality.emplace_back(param.input_file,
                                param.traces_file, actualQuery, actualQuery.size(),
                                casus, noise, rankingDistance, spearman, proposedMetric, false,
                                param.tuning_factor,
                                param.use_path_with_lambda_factor,
                                param.lambda,
                                param.max_length,
                                param.min_prob);
    {
        expectedRanking = doDulcior(expectedRanking, pathsOrder.size());
        pathRanking = doDulcior(pathRanking, pathsOrder.size());
        double rankingDistance = expectedRanking.normalizedRank(pathRanking, ls);
        double spearman = expectedRanking.SpearmanCorrelationIndex(pathRanking, 1.0);
        double proposedMetric = minimum_edit_maximum_substring(pathRanking, expectedRanking);

        output_quality.emplace_back(param.input_file,
                                    param.traces_file, actualQuery, actualQuery.size(),
                                    casus, noise, rankingDistance, spearman, proposedMetric, true,
                                    param.tuning_factor,
                                    param.use_path_with_lambda_factor,
                                    param.lambda,
                                    param.max_length,
                                    param.min_prob);
        size_t i = 0;
        i++;
    }
}

#include <embeddings/path_embedding/EmbedPahtsOverSingleGraphStrategy.h>

MultiplePathsEmbeddingStrategy *MultithreadedBenchmarkForPooling::generatePathEmbeddingStrategyFromParameters() const {
    switch (casus) {
        case ONLY_EDGE_INFORMATION_PROPOSED:
            return new EmbedPathsOverSingleGraphStrategy<OnlyTransitiveEdgesCost<true>>(param.use_path_with_lambda_factor, epsilon, param.tuning_factor, param.lambda, false, param.max_length, param.min_prob);
        case EDGE_WITH_NODE_INFORMATION_PROPOSED:
            return new EmbedPathsOverSingleGraphStrategy<NodesWithTransitiveEdgeCost<true>>(param.use_path_with_lambda_factor, epsilon, param.tuning_factor, param.lambda, false, param.max_length, param.min_prob);
        case ONLY_EDGE_INFORMATION_PREVIOUS:
            return new EmbedPathsOverSingleGraphStrategy<OnlyTransitiveEdgesCost<false>>(param.use_path_with_lambda_factor, epsilon, param.tuning_factor, param.lambda, false, param.max_length, param.min_prob);
        case EDGE_WITH_NODE_INFORMATION_PREVIOUS:
            return new EmbedPathsOverSingleGraphStrategy<NodesWithTransitiveEdgeCost<false>>(param.use_path_with_lambda_factor, epsilon, param.tuning_factor, param.lambda, false, param.max_length, param.min_prob);
    }
}

#include <embeddings/graph_embedding/TransitiveClosureGraphStrategy.h>

GraphEmbeddingStrategy *
MultithreadedBenchmarkForPooling::generateGraphEmbeddingStrategyFromParameters() const {
    switch (casus) {
        case ONLY_EDGE_INFORMATION_PROPOSED:
            return new TransitiveClosureGraphStrategy<OnlyTransitiveEdgesCost<true>>(param.use_path_with_lambda_factor, epsilon, param.tuning_factor, param.lambda, param.max_length);
        case EDGE_WITH_NODE_INFORMATION_PROPOSED:
            return new TransitiveClosureGraphStrategy<NodesWithTransitiveEdgeCost<true>>(param.use_path_with_lambda_factor, epsilon, param.tuning_factor, param.lambda, param.max_length);
        case ONLY_EDGE_INFORMATION_PREVIOUS:
            return new TransitiveClosureGraphStrategy<OnlyTransitiveEdgesCost<false>>(param.use_path_with_lambda_factor, epsilon, param.tuning_factor, param.lambda,  param.max_length);
        case EDGE_WITH_NODE_INFORMATION_PREVIOUS:
            return new TransitiveClosureGraphStrategy<NodesWithTransitiveEdgeCost<false>>(param.use_path_with_lambda_factor, epsilon, param.tuning_factor, param.lambda, param.max_length);
    }
}

double MultithreadedBenchmarkForPooling::set_of_path_similarity(const std::vector<path_info> &tracesSet,
                                                                const std::vector<path_info> &querySet,
                                                                const LevensteinSimilarity &sim,
                                                                std::vector<additional_benchmarks_per_log> *opt,
                                                                double *precisionNormalization,
                                                                PathEmbeddingStrategy *strategy) {
    double cost = 0.0;
    for (const auto& x : tracesSet) {
        for (const auto& y : querySet) {
            double val = sim.similarity(x.path, y.path)*(x.probability*y.probability);
            if (opt && precisionNormalization && strategy) {
                opt->emplace_back(param.input_file, param.traces_file, x.path, x.path.size(), *strategy, 0.0,  false, param.tuning_factor, param.use_path_with_lambda_factor, param.lambda, param.max_length, param.min_prob, LogPrecisionMetric, val/(*precisionNormalization), y.path, y.path.size());
            }
            cost += val;
        }
    }
    return cost;
}

std::pair<std::vector<record_element_per_query>, std::vector<additional_benchmarks_per_log>>  MultithreadedBenchmarkForPooling::run() {
    if (!pathstrategy) pathstrategy = generatePathEmbeddingStrategyFromParameters();
    if (!graphStrategy) graphStrategy = generateGraphEmbeddingStrategyFromParameters();
    std::set<std::pair<std::string, std::string>> embedding_space;
    std::cout << "Strategy " << magic_enum::enum_name<PathEmbeddingStrategy>(casus).data() << std::endl;


    std::cout << " * generating embeddings" << std::flush;
    ReadGraph::path_to_uembedding ptg;
    ReadGraph::path_to_uembedding ptu = (*pathstrategy)(copyGraph);
    ReadGraph::extractEmbeddingSpace(embedding_space, ptu);
    auto map = ReadGraph::generateStructuredEmbeddings(embedding_space, ptu);
    std::vector<struct path_info> mapPath;
    for (const auto &path : map) {
        mapPath.emplace_back(path.first);
    }
    std::cout << "... done" << std::endl;

    std::cout << " * generation precision metric" << std::flush;
    double precision_normalization = std::sqrt(set_of_path_similarity(mapPath, mapPath, similarity) *
                                               set_of_path_similarity(finalOriginalLog, finalOriginalLog,
                                                                      similarity));
    double precision =
            set_of_path_similarity(mapPath, finalOriginalLog, similarity, &log_quality, &precision_normalization) /
            precision_normalization;
    std::cout << "... done" << std::endl;
    std::cout << "   precision = " << precision << std::endl;

    std::cout << " * starting with query analysis!" << std::endl;
    for (const auto &query : finalLog) {
        std::string actualQuery = query.path;
        double noise = 0.0;
        std::cout << "    - trace: " << actualQuery << std::endl;

        performBenchmark2(embedding_space, map, actualQuery, noise, mapPath);

        for (auto &dbl : noiseThreshold) {
            AlterString noiser{charList, dbl, seed};
            std::string alteredQuery;
            do {
                if (alteredQuery == "bcc")
                    std::cerr << "DEBUG" << std::endl;
                alteredQuery = noiser.alter(query.path);
            } while ((alteredQuery == query.path) || (alteredQuery.empty()));
            std::cout << "      : alteredAs: " << alteredQuery << std::endl;

            performBenchmark2(embedding_space, map, actualQuery, dbl, mapPath);
        }
    }
    std::cout << "Phase done!" << std::endl;

    return {this->output_quality, this->log_quality};
}

MultithreadedBenchmarkForPooling::MultithreadedBenchmarkForPooling(size_t threadIdForLogging,
                                                                   PathEmbeddingStrategy casus,
                                                                   const std::string &epsilon,
                                                                   const benchmark_common_ground &param,
                                                                   DistanceExpressionEvaluator *probSimMetric,
                                                                   const ReadGraph &copyGraph,
                                                                   const std::string &charList, size_t seed,
                                                                   std::vector<double> &noiseThreshold,
                                                                   std::vector<struct path_info> &finalLog,
                                                                   std::vector<struct path_info> &finalOriginalLog)
        : thread_id_for_logging(threadIdForLogging), casus(casus), epsilon(epsilon), param(param),
          probSimMetric(probSimMetric), copyGraph(copyGraph), charList(charList), seed(seed),
          noiseThreshold(noiseThreshold), finalLog(finalLog), finalOriginalLog(finalOriginalLog) {}
