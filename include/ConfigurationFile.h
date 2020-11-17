//
// Created by giacomo on 11/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_INPUTFORMAT_H
#define FUZZYSTRINGMATCHING2_INPUTFORMAT_H

#include <string>
#include <distances/WeightEstimator.h>
#include <chrono>
#include <iostream>
#include <vector>
#include <ReadGraph.h>
#include <data_loading/load_data.h>
#include <log/log_operations.h>

#include <utils/fixed_bimap.h>
#include <filesystem>
#include <topk/topk.h>
#include <utils/AlterString.h>
#include <memory>
#include <embeddings/path_embedding/MultiplePathsEmbeddingStrategy.h>
#include <embeddings/graph_embedding/GraphEmbeddingStrategy.h>
#include <distances/strings/LevensteinSimilarity.h>

class ExpressionEvaluator;

struct record_element {
    std::string input_file;
    std::string traces_file;
    std::string query;
    size_t query_length;
    PathEmbeddingStrategy embeddingStrategy;
    double trace_noise;

    double tuning_factor;
    bool   use_path_with_lambda_factor;
    double lambda;
    size_t max_length;
    double min_prob;

    double rankingDistance;
    double spearman;
    double proposedMetric;
    double precision;
    double generalizationScoreForQuery;
    std::string with_trace;

    bool   dulcior;

    record_element(const std::string &inputFile, const std::string &tracesFile,
                   const std::string &query, size_t queryLength,
                   PathEmbeddingStrategy embeddingStrategy, double traceNoise, double rankingDistance,
                   double spearman, double proposedMetric, bool dulcior,
                   double generalizationScoreForQuery, std::string withTrace, double tuningFactor,
                   bool usePathWithLambdaFactor, double lambda, size_t maxLength, double minProb, double precision);
};

struct ConfigurationFile {
    /**
     * Loading the class from a configuration file, if it exists. Otherwise, the default parameters are going to be loaded
     * @param filename
     */
    ConfigurationFile(const std::string& filename);

    char  varepsilon = '.';
    std::string  admissibleCharList{"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"};
    std::vector<double> noiseThreshold        = {0.3};
    size_t seedError             = 0;

    std::filesystem::path results_folder; // The folder where all the temporarily results are produced

    ReadGraph    finalGraph;
    FileFormat   input_file_format      = PetriMatrix;
    std::string  input_file             = "data/petri_matrix.txt";
    bool         is_input_compressed    = false;
    size_t       ith_graph              = 0;

    std::unordered_map<UnterstuetzenStrategie, std::string> fileStrategyMap;
    std::unordered_map<UnterstuetzenStrategie, ExpressionEvaluator*> fileStrategyMap_loaded; // I am forced to use a pointer because of the includion interference error from antlr4, and therefore I need to use an explicit destructor

    TracesFormat trace_file_format              = TracesFormat::NoLog;
    std::string  traces_file;
    bool         are_traces_compressed          = false;
    char         separator_if_any               = ';';
    std::vector<LogOperationConfiguration> operations;
    std::vector<struct path_info>                 finalLog;
    std::vector<struct path_info>                 finalOriginalLog;

    bool         add_traces_to_log              = true;
    size_t       max_length                     = std::numeric_limits<size_t>::max();
    double       min_prob                       = std::numeric_limits<double>::epsilon()*2;
    double       lambda                         = 0.7;
    bool use_tuning_factor                      = false;
    bool use_path_lambda_factor                 = false;
    double       tuning_factor                  = -1.0;

    bool                         use_estimator;
    spd_we::WeightEstimatorCases estimator_type = spd_we::W_CONSTANT;

    fixed_bimap<std::string, char>   action_to_single_char;

    std::vector<AlterString> traceNoiser;
    size_t                   numberOfTraceAlterations = 10;

    /**
     * Generate edge embedding strategy from the defined PathEmbeddingStrategy
     * @param casus
     * @return              allocated strategy
     */
    MultiplePathsEmbeddingStrategy* generatePathEmbeddingStrategyFromParameters(enum PathEmbeddingStrategy casus) const ;

    GraphEmbeddingStrategy*         generateGraphEmbeddingStrategyFromParameters(enum PathEmbeddingStrategy casus) const;
    void run();
    void serialize(const std::string& file = "");

    ~ConfigurationFile();

private:
    std::string configuration_filename;

    void
    performBenchmark(const LevensteinSimilarity &similarity, ExpressionEvaluator *probSimMetric,
                     std::vector<record_element> &output_quality,
                     std::set<std::pair<std::string, std::string>> &embedding_space, PathEmbeddingStrategy &strategy,
                     GraphEmbeddingStrategy *graphStrategy,
                     const std::unordered_map<struct path_info, Eigen::VectorXd> &map, std::string &actualQuery,
                     double noise,
                     std::vector<struct path_info>& pathsOrder, double precision) const;

    void
    convertLog(const std::vector<Transaction<std::string>> &currentLog, std::vector<struct path_info> &final);
};

bool isFileFormatPetri(enum FileFormat format);


#include <benchmarking/Ranking.h>

/*template <typename T, typename K> Ranking<K> dulciorRanking(const std::unordered_map<struct path_info, T>& map, Ranking<K>& rank, bool dulcior) {
    if (!dulcior) {
        return rank;
    } else {
        Ranking<K> finalRanking;
        for (const auto& y: map) {
            finalRanking.addScore(y.first, rank.getRanking(y.first));
        }
        return finalRanking;
    }
}*/



#endif //FUZZYSTRINGMATCHING2_INPUTFORMAT_H
