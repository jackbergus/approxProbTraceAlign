//
// Created by giacomo on 11/11/20.
//

#include "ConfigurationFile.h"
#include <regex>
#include <GenericGraph.h>
#include <distances/WeightEstimator.h>
#include <log/log_operations.h>
#include <iostream>
#include <topk/Aussageform/ExpressionEvaluator.h> //--> DO NOT INCLUDE IN THE .H! For this reason, I opted out

#define READ_GRAPH(format)      std::cout << "Reading graph '" << this->input_file << "' as a " << format << " file. Getting the " << this->ith_graph << "-th graph" << std::endl

#include <magic_enum.hpp>
#include <topk/old/minimum_edit_maximum_substring.h>
#include <distances/strings/LevensteinSimilarity.h>


double score(const Eigen::VectorXd& left, const Eigen::VectorXd& right) {
    return left.dot(right);
}


double set_of_path_similarity(const std::vector<path_info>& left, const std::vector<path_info>& right, const LevensteinSimilarity& sim) {
    double cost = 0.0;
    for (const auto& x : left)
        for (const auto& y : right)
            cost += sim.similarity(x.path, y.path);
    return cost;
}


void ConfigurationFile::run() {

    {
        std::ostringstream  oss;
        std::time_t t = std::time(nullptr);
        std::tm tm = *std::localtime(&t);
        oss << std::put_time(&tm, "%F %T%z - %a %e %b, %Y") << std::endl;
        this->results_folder = oss.str();
        std::filesystem::create_directory(results_folder);
        serialize((results_folder / "configuration.yaml").c_str());
    }

    GenericGraph<size_t> graph;
    spd_we::WeightEstimator<size_t> we;
    we.setVarEpsilon({varepsilon});
    std::vector<Transaction<std::string>> logFromFile, originalLog;
    admissibleCharList.erase(std::remove(admissibleCharList.begin(), admissibleCharList.end(), varepsilon), admissibleCharList.end());
    std::string epsilon{this->varepsilon};
    LevensteinSimilarity similarity;

    switch (this->input_file_format) {
        case Petri_PNML:
            READ_GRAPH("PNML (Stochastic Petri Net)");
            graph = load_pnml(this->input_file, epsilon)[this->ith_graph];
            break;

        case Petri_BPMN:
            READ_GRAPH("Simple BPMN (to Stochastic Petri Net)");
            graph = load_bpmn(this->input_file, epsilon)[this->ith_graph];
            break;

        case PetriMatrix:
            READ_GRAPH("weighted- and labelled- node matrix");
            graph = load_petri_matrix(this->input_file);
            break;

        case StochasticMatrix: // TODO
            READ_GRAPH("node-labelled and edge-weigthed (stochastic) matrix");
            graph = load_matrix(this->input_file);
            break;

        case ProbRegex: // TODO
            READ_GRAPH("probabilistic REGEX");
            graph = load_pregex(this->input_file, epsilon);
            break;
    }
    graph.render((this->results_folder / "graph_01_input.pdf").c_str());
    bool rememberToLog = false;
    bool useEstimator = false;


    if (isFileFormatPetri(this->input_file_format)) {
        if (!this->use_estimator) {
            std::cout << "Using no estimator: no traces are going to be loaded!" << std::endl;
            if (this->trace_file_format != NoLog) {
                std::cerr << "Ignoring the log file: no estimator is used" << std::endl;
                this->trace_file_format = NoLog;
            }
        } else {
            if (this->estimator_type == spd_we::W_CONSTANT) {
                std::cout << "Using the constant estimator" << std::endl;
                std::cerr << "Ignoring the log file: the constant estimator will be used" << std::endl;
                this->trace_file_format = NoLog;
            } else {
                std::cout << "Using an estimator: a log file will be loaded" << std::endl;
                useEstimator = true;
            }

            switch (this->trace_file_format) {
                case XESLog:
                    logFromFile = load_xes(this->traces_file);
                    originalLog = logFromFile;
                    performLogOperation(this->operations, logFromFile);
                    rememberToLog = true;
                    break;
                case RawLog:
                    logFromFile = read_log(this->traces_file, this->separator_if_any);
                    originalLog = logFromFile;
                    performLogOperation(this->operations, logFromFile);
                    rememberToLog = true;
                    break;

                case NoLog:
                    std::cerr << "No trace file is provided: either the a constant weight or the pre-loaded weight will be set"  << std::endl;
                    break;
            }
        }


        std::cout << "Trasforming the PetriNet (IS) into a Thompson automaton [transferring the edges from the nodes to the transitions]... " << std::flush;
        {
            auto t1 = std::chrono::high_resolution_clock::now();
            graph.transfer_weight_from_nodes_to_edges();
            auto now = std::chrono::high_resolution_clock::now();
            std::cout << " done (" << std::chrono::duration_cast<std::chrono::nanoseconds>(now-t1).count() << " ns)" << std::endl;
            graph.render((this->results_folder / "graph_02_weight_transfer.pdf").c_str());
        }
    }

    std::cout << "Performing the varepsilon-closure... " << std::flush;
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        graph.doClosure(epsilon);
        auto now = std::chrono::high_resolution_clock::now();
        std::cout << " done (" << std::chrono::duration_cast<std::chrono::nanoseconds>(now-t1).count() << " ns)" << std::endl;
        graph.render((this->results_folder / "graph_03_e-closed.pdf").c_str());
    }


    if (rememberToLog && useEstimator) {
        auto t1 = std::chrono::high_resolution_clock::now();
        std::cout << "Retrieving the estimator phase after the varepsilon closure... " << std::flush;
        we.setGraph(&graph);
        we.setLog(logFromFile);
        for (const auto& id : graph.getNodes()) {
            graph.updateNodeWeight(id, we.getNodeWeight(id, this->estimator_type));
        }
        graph.render((this->results_folder / "graph_04_estimator.pdf").c_str());
        graph.transfer_weight_from_nodes_to_edges();
        graph.render((this->results_folder / "graph_04_estimator_transfer.pdf").c_str());
        auto now = std::chrono::high_resolution_clock::now();
        std::cout << " done (" << std::chrono::duration_cast<std::chrono::nanoseconds>(now-t1).count() << " ns)" << std::endl;
    }

    graph.render();

    std::cout << "Converting to the simple benchmark graph" << std::endl;
    std::cout << " 1) Initializing the final graph" << std::endl;
    finalGraph.init(graph.nodes()+1, graph.countEdges(), graph.getStart(), graph.getEnd());
    finalGraph.name = graph.getName();

    std::cout << " 2) Generating the correspondence full_action_name <-> char" << std::endl;
    action_to_single_char.put(epsilon, varepsilon);
    graph.generateBimapLabels(this->action_to_single_char, this->admissibleCharList, epsilon);
    std::cout << "The map was generated as follows: " <<std::endl;
    for (const auto& cp : action_to_single_char.getElements()) {
        std::cout<< "  - " << cp.first << " <-> " << cp.second << std::endl;
    }

    std::cout << " 3) Filling up the elements" << std::endl;
    for (const auto& n : graph.getNodes()) {
        finalGraph.addNode(n, {this->action_to_single_char.getValue(graph.getNodeLabel(n))});
        for (const auto& e : graph.outgoing(n)) {
            finalGraph.addEdge(n, e.first, e.second);
        }
    }
    finalGraph.finalizeEdgesMatrix(graph.getCost());

    std::cout << " 4) Converting the traces to single strings via correspondence" << std::endl;
    convertLog(logFromFile, finalLog);
    convertLog(originalLog, finalOriginalLog);


    if (this->add_traces_to_log) {
        std::cout << "5) Adding some further traces to the log from the generated paths. Settings: maxLength = " << this->max_length << " minProb = " << this->min_prob << std::endl;
        for (const auto& path : finalGraph.iterateOverPaths(false, max_length, min_prob)) {
            std::cout << " New trace = '" << path.path << "' with probability = " << path.probability << std::endl;
            finalLog.emplace_back(path);
        }
    }

    constexpr auto& color_entries =  magic_enum::enum_entries<PathEmbeddingStrategy>();
    constexpr std::size_t color_count = magic_enum::enum_count<PathEmbeddingStrategy>();
    ExpressionEvaluator *probSimMetric = fileStrategyMap_loaded[UnterstuetzenStrategie::ProbabilitySimilarity];

    std::vector<record_element> output_quality;

    for (size_t i = 0; i<color_count; i++) {
        std::set<std::pair<std::string,std::string>> embedding_space;
        PathEmbeddingStrategy strategy = color_entries[i].first;
        MultiplePathsEmbeddingStrategy* pathstrategy  = generatePathEmbeddingStrategyFromParameters(strategy);
        GraphEmbeddingStrategy *        graphStrategy = generateGraphEmbeddingStrategyFromParameters(strategy);

        ReadGraph::path_to_uembedding ptg;
        ReadGraph::path_to_uembedding ptu = (*pathstrategy)(finalGraph);
        ReadGraph::extractEmbeddingSpace(embedding_space, ptu);
        auto map = ReadGraph::generateStructuredEmbeddings(embedding_space, ptu);
        std::vector<struct path_info> mapPath;
        for (const auto& path : map) {
            mapPath.emplace_back(path.first);
        }

        double precision = set_of_path_similarity(mapPath, finalOriginalLog, similarity)/std::sqrt(set_of_path_similarity(mapPath, mapPath, similarity)*set_of_path_similarity(finalOriginalLog, finalOriginalLog, similarity));

        for (const auto& query : finalLog) {
            std::string actualQuery = query.path;
            double noise = 0.0;

            performBenchmark(similarity, probSimMetric, output_quality, embedding_space, strategy,
                                               graphStrategy, map, actualQuery, noise, mapPath, precision);

            for (auto& noiser : traceNoiser) {
                std::string alteredQuery = noiser.alter(query.path);

                performBenchmark(similarity, probSimMetric, output_quality, embedding_space, strategy,
                                                   graphStrategy, map, actualQuery, noiser.noiseThreshold, mapPath, precision);
            }
        }


        delete graphStrategy;
        delete pathstrategy;
    }

}

void ConfigurationFile::convertLog(const std::vector<Transaction<std::string>> &currentLog,
                                   std::vector<struct path_info> &final)  {
    for (const auto& trace : currentLog) {
        std::string stringBuilder;
        for (const auto& str : trace) {
            stringBuilder += action_to_single_char.getValue(str);
        }
        final.emplace_back(1.0, stringBuilder, std::vector<size_t>{});
    }
}

Ranking<size_t> doDulcior(Ranking<size_t>& orig, size_t N) {
    Ranking<size_t> finalRanking;
    for (size_t i = 0; i<N; i++) {
        finalRanking.addScore(i, orig.getRanking(i));
    }
    return finalRanking;
}

void
ConfigurationFile::performBenchmark(const LevensteinSimilarity &similarity, ExpressionEvaluator *probSimMetric,
                                    std::vector<record_element> &output_quality,
                                    std::set<std::pair<std::string, std::string>> &embedding_space,
                                    PathEmbeddingStrategy &strategy, GraphEmbeddingStrategy *graphStrategy,
                                    const std::unordered_map<struct path_info, Eigen::VectorXd> &map,
                                    std::string &actualQuery, double noise,
                                    std::vector<struct path_info>& pathsOrder, double precision) const {
    ReadGraph g = ReadGraph::fromString(actualQuery, 1.0);
    auto tmp = (*graphStrategy)(g);
    auto x = ReadGraph::generateStructuredEmbedding(embedding_space, tmp);
    Ranking<size_t> pathRanking, expectedRanking;
    std::map<double, std::vector<size_t>> rankingMap;
    size_t j = 0;
    double forGeneralization = -1.0;
    size_t genIdx = 0;
    for (const auto& paths : map) {
        double sc = score(x, paths.second);
        pathRanking.addScore(j, sc);
        double similarity = 1.0/(GeneralizedLevensteinDistance(paths.first.path, actualQuery)/5.0+1.0);
        if (similarity > forGeneralization) {
            genIdx = j;
            forGeneralization = similarity;
        }

        double finalScore = probSimMetric->operator()({paths.first.probability, });
        expectedRanking.addScore(j, finalScore);
        rankingMap[finalScore].emplace_back(j);

        j++;
    }
    ///for (const auto& x : rankingMap) assert(x.second.size() == 1); // Imposing that there are no elements with the same scoring value
    const auto ls = [&similarity, &pathsOrder](const size_t& left, const size_t& right) { return similarity.similarity(pathsOrder.at(left).path, pathsOrder.at(right).path); };
    double rankingDistance = expectedRanking.normalizedRank(pathRanking, ls);
    double spearman = expectedRanking.SpearmanCorrelationIndex(pathRanking, 1.0);
    double proposedMetric = minimum_edit_maximum_substring(pathRanking, expectedRanking);

    output_quality.emplace_back(input_file, traces_file, actualQuery, actualQuery.size(), strategy, noise, rankingDistance, spearman, proposedMetric, false, forGeneralization, pathsOrder[genIdx].path, tuning_factor, use_path_lambda_factor, lambda, max_length, min_prob, precision);
    {


        expectedRanking = doDulcior(expectedRanking, pathsOrder.size());
        pathRanking = doDulcior(pathRanking, pathsOrder.size());
        double rankingDistance = expectedRanking.normalizedRank(pathRanking, ls);
        double spearman = expectedRanking.SpearmanCorrelationIndex(pathRanking, 1.0);
        double proposedMetric = minimum_edit_maximum_substring(pathRanking, expectedRanking);

        output_quality.emplace_back(input_file, traces_file, actualQuery, actualQuery.size(), strategy, noise, rankingDistance, spearman, proposedMetric, true, forGeneralization, pathsOrder[genIdx].path, tuning_factor, use_path_lambda_factor, lambda, max_length, min_prob, precision);
    }
}

#include <yaml-cpp/yaml.h>
#include <magic_enum.hpp>
#include <fstream>

#define ENUM_SERIALIZE(arg)             YAML::Key << #arg << YAML::Value << (magic_enum::enum_name(arg).data())
#define SIMPLE_SERIALIZE(arg)           YAML::Key << #arg << YAML::Value << arg
#define STRING_SERIALIZE(arg)           SIMPLE_SERIALIZE(arg)
#define BOOL_SERIALIZE(arg)             SIMPLE_SERIALIZE(arg)
#define INT_SERIALIZE(arg)              SIMPLE_SERIALIZE(arg)
#define DBL_SERIALIZE(arg)              SIMPLE_SERIALIZE(arg)

void ConfigurationFile::serialize(const std::string& filename) {
    YAML::Emitter out;
    out.SetIndent(4);
    out << YAML::BeginMap;

    out << ENUM_SERIALIZE(input_file_format);
    out << STRING_SERIALIZE(input_file);
    out << BOOL_SERIALIZE(is_input_compressed);
    out << INT_SERIALIZE(ith_graph);

    out << ENUM_SERIALIZE(trace_file_format);
    out << STRING_SERIALIZE(traces_file);
    out << BOOL_SERIALIZE(are_traces_compressed);
    out << STRING_SERIALIZE(separator_if_any);

    out << BOOL_SERIALIZE(add_traces_to_log);
    out << INT_SERIALIZE(max_length);
    out << DBL_SERIALIZE(min_prob);
    out << DBL_SERIALIZE(lambda);
    out << DBL_SERIALIZE(tuning_factor);
    use_tuning_factor = (tuning_factor >= 0.0) && use_tuning_factor;
    out << BOOL_SERIALIZE(use_tuning_factor);
    out << BOOL_SERIALIZE(use_path_lambda_factor);
    out << INT_SERIALIZE(numberOfTraceAlterations);


    out << BOOL_SERIALIZE(use_estimator);
    out << ENUM_SERIALIZE(estimator_type);

    if (!operations.empty()) {
        out << YAML::Key << "operations" << YAML::Value;
        out << YAML::BeginSeq;
        for (const auto& arg : operations) {
            const auto operation = arg.operation;
            const auto factor    = arg.factor;
            const auto keep_low_up_otherwise = arg.keep_low_up_otherwise;
            out << YAML::BeginMap;
            out << ENUM_SERIALIZE(operation);
            out << DBL_SERIALIZE(factor);
            out << BOOL_SERIALIZE(keep_low_up_otherwise);
            out << YAML::EndMap;
        }
        out << YAML::EndSeq;
    }
    if (!fileStrategyMap.empty()) {
        out << YAML::Key << "fileStrategyMap" << YAML::Value;
        out << YAML::BeginMap;
        for (const auto& cp : fileStrategyMap) {
            out << YAML::Key << (magic_enum::enum_name(cp.first).data()) << YAML::Value << cp.second;
        }
        out << YAML::EndMap;
    }

    out << STRING_SERIALIZE(varepsilon);
    out << STRING_SERIALIZE(admissibleCharList);
    if (!noiseThreshold.empty()) {
        out << YAML::Key << "noiseThreshold" << YAML::Value << YAML::BeginSeq;
        for (const auto& x: noiseThreshold) {
            out << x;
        }
        out << YAML::EndSeq;
    }
    //out << DBL_SERIALIZE(noiseThreshold);
    out << INT_SERIALIZE(seedError);

    out << YAML::EndMap;

    {
        if (filename.empty()) {
            std::ofstream file(configuration_filename);
            file << out.c_str();
        } else {
            std::ofstream file(filename);
            file << out.c_str();
        }

    }
}

#include <sys/stat.h>
#include <embeddings/labelled_paths/NodesWithTransitiveEdgeCost.h>

inline bool exists_test3 (const std::string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

#define PARSE_ENUM_EXT(str, arg, etype)                                                              \
    {                                                                                       \
        auto it = config[ str ];                                                           \
        if (it) {                                                                           \
            auto tentativeEnum = magic_enum::enum_cast<etype>(it.as<std::string>());        \
            if (tentativeEnum.has_value()) {                                                \
                arg = tentativeEnum.value();                                                \
            }                                                                               \
        }                                                                                   \
    }



#define PARSE_ENUM(arg, etype)         PARSE_ENUM_EXT( #arg, arg, etype)                                                  /*   \
    {                                                                                       \
        auto it = config[ #arg ];                                                           \
        if (it) {                                                                           \
            auto tentativeEnum = magic_enum::enum_cast<etype>(it.as<std::string>());        \
            if (tentativeEnum.has_value()) {                                                \
                arg = tentativeEnum.value();                                                \
            }                                                                               \
        }                                                                                   \
    }*/

#define PARSE_STRING(arg)                       \
    {                                           \
        auto it = config[ #arg ];               \
        if (it) {                               \
            arg = it.as<std::string>();         \
        }                                       \
    }                                           \

#define PARSE_CHAR(arg)                         \
    {                                           \
        auto it = config[ #arg ];               \
        if (it) {                               \
            arg = it.as<std::string>()[0];      \
        }                                       \
    }                                           \

#define PARSE_INT(arg)                          \
    {                                           \
        auto it = config[ #arg ];               \
        if (it) {                               \
            arg = it.as<size_t>();              \
        }                                       \
    }                                           \

#define PARSE_DBL_EXT(str, arg)                          \
    {                                           \
        auto it = config[ str ];               \
        if (it) {                               \
            arg = it.as<double>();              \
        }                                       \
    }                                           \


#define PARSE_DBL(arg)     PARSE_DBL_EXT( #arg, arg)

#define PARSE_BOOL_EXT(str, arg)                          \
    {                                           \
        auto it = config[ str ];               \
        if (it) {                               \
            arg = it.as<bool>();              \
        }                                       \
    }                                           \

#define PARSE_BOOL(arg)       PARSE_BOOL_EXT( #arg, arg )

ConfigurationFile::ConfigurationFile(const std::string &filename) : configuration_filename{filename} {
    if (exists_test3(configuration_filename)) {
        // Perform the parsing if and only if the configuration file exists
        YAML::Node config = YAML::LoadFile(configuration_filename);

        PARSE_ENUM(input_file_format, FileFormat);
        PARSE_STRING(input_file);
        PARSE_INT(ith_graph);
        PARSE_BOOL(is_input_compressed);

        PARSE_ENUM(trace_file_format, TracesFormat);
        PARSE_STRING(traces_file);
        PARSE_BOOL(are_traces_compressed);
        PARSE_CHAR(separator_if_any);

        PARSE_BOOL(add_traces_to_log);
        PARSE_INT(max_length);
        PARSE_INT(numberOfTraceAlterations);
        PARSE_DBL(min_prob);
        PARSE_DBL(lambda);
        PARSE_DBL(tuning_factor);
        PARSE_BOOL(use_tuning_factor);
        PARSE_BOOL(use_path_lambda_factor);
        if (!use_tuning_factor) tuning_factor = -1.0;

        PARSE_BOOL(use_estimator);
        PARSE_ENUM(estimator_type, spd_we::WeightEstimatorCases);

        PARSE_STRING(admissibleCharList);
        {
            auto it = config["noiseThreshold"];
            noiseThreshold.clear();
            if ((it) && (it.IsSequence())) {
                for (const auto& x : it) {
                    noiseThreshold.emplace_back(x.as<double>());
                    traceNoiser.emplace_back(admissibleCharList, *noiseThreshold.rbegin(), seedError);
                }
            }
        }
        ///PARSE_DBL("noiseThreshold");
        PARSE_CHAR(varepsilon);
        PARSE_INT(seedError);
        ///traceNoiser = AlterString{admissibleCharList, noiseThreshold, seedError}; // Setting the trace noiser from the default arguments

        {
            const auto operationList = config["operations"];
            if ((operationList) && (operationList.IsSequence())) {
                for (const auto & i : operationList) {
                    const auto& config = i;

                    LogOperationConfiguration arg;
                    PARSE_ENUM_EXT("operation", arg.operation, LogOperations);
                    PARSE_DBL_EXT("factor", arg.factor);
                    PARSE_BOOL_EXT("keep_low_up_otherwise", arg.keep_low_up_otherwise);
                    this->operations.emplace_back(arg);
                }
            }
        }

        {
            const auto strategy = config["fileStrategyMap"];
            if ((strategy) && (strategy.IsMap())) {
                for (auto& kv : strategy) {
                    assert(kv.first.IsScalar() && kv.second.IsScalar());
                    UnterstuetzenStrategie arg;
                    auto tentativeEnum = magic_enum::enum_cast<UnterstuetzenStrategie>(kv.first.as<std::string>());
                    if (tentativeEnum.has_value()) {
                        arg = tentativeEnum.value();
                        fileStrategyMap.emplace(arg, kv.second.as<std::string>());
                        auto ptr = new ExpressionEvaluator(kv.second.as<std::string>());
                        ptr->setStrategy(arg);
                        fileStrategyMap_loaded.emplace(arg, ptr);
                    }
                }
            }
        }
    }

}

ConfigurationFile::~ConfigurationFile() {
    if (!fileStrategyMap_loaded.empty()) for (auto &cp : fileStrategyMap_loaded) delete cp.second; // deallocating all the pointers
}

#include <embeddings/path_embedding/EmbedPahtsOverSingleGraphStrategy.h>
#include <embeddings/graph_embedding/TransitiveClosureGraphStrategy.h>

MultiplePathsEmbeddingStrategy *
ConfigurationFile::generatePathEmbeddingStrategyFromParameters(enum PathEmbeddingStrategy casus) const {
    std::string epsilon{varepsilon};
    switch (casus) {
        case ONLY_EDGE_INFORMATION_PROPOSED:
            return new EmbedPathsOverSingleGraphStrategy<OnlyTransitiveEdgesCost<true>>(this->use_path_lambda_factor, epsilon, tuning_factor, lambda, false, max_length, min_prob);
        case EDGE_WITH_NODE_INFORMATION_PROPOSED:
            return new EmbedPathsOverSingleGraphStrategy<NodesWithTransitiveEdgeCost<true>>(this->use_path_lambda_factor, epsilon, tuning_factor, lambda, false, max_length, min_prob);
        case ONLY_EDGE_INFORMATION_PREVIOUS:
            return new EmbedPathsOverSingleGraphStrategy<OnlyTransitiveEdgesCost<false>>(this->use_path_lambda_factor, epsilon, tuning_factor, lambda, false, max_length, min_prob);
        case EDGE_WITH_NODE_INFORMATION_PREVIOUS:
            return new EmbedPathsOverSingleGraphStrategy<NodesWithTransitiveEdgeCost<false>>(this->use_path_lambda_factor, epsilon, tuning_factor, lambda, false, max_length, min_prob);
    }
}

GraphEmbeddingStrategy *
ConfigurationFile::generateGraphEmbeddingStrategyFromParameters(enum PathEmbeddingStrategy casus) const {
    std::string epsilon{varepsilon};
    switch (casus) {
        case ONLY_EDGE_INFORMATION_PROPOSED:
            return new TransitiveClosureGraphStrategy<OnlyTransitiveEdgesCost<true>>(this->use_path_lambda_factor, epsilon, tuning_factor, lambda, max_length);
        case EDGE_WITH_NODE_INFORMATION_PROPOSED:
            return new TransitiveClosureGraphStrategy<NodesWithTransitiveEdgeCost<true>>(this->use_path_lambda_factor, epsilon, tuning_factor, lambda, max_length);
        case ONLY_EDGE_INFORMATION_PREVIOUS:
            return new TransitiveClosureGraphStrategy<OnlyTransitiveEdgesCost<false>>(this->use_path_lambda_factor, epsilon, tuning_factor, lambda,  max_length);
        case EDGE_WITH_NODE_INFORMATION_PREVIOUS:
            return new TransitiveClosureGraphStrategy<NodesWithTransitiveEdgeCost<false>>(this->use_path_lambda_factor, epsilon, tuning_factor, lambda, max_length);
    }
}

record_element::record_element(const std::string &inputFile, const std::string &tracesFile,
                               const std::string &query, size_t queryLength,
                               PathEmbeddingStrategy embeddingStrategy, double traceNoise, double rankingDistance,
                               double spearman, double proposedMetric, bool dulcior,
                               double generalizationScoreForQuery, std::string withTrace, double tuningFactor,
                               bool usePathWithLambdaFactor, double lambda, size_t maxLength, double minProb, double precision)
        : input_file(inputFile), traces_file(tracesFile), query(query), query_length(queryLength),
          embeddingStrategy(embeddingStrategy), trace_noise(traceNoise), rankingDistance(rankingDistance),
          spearman(spearman), proposedMetric(proposedMetric), dulcior(dulcior),
          generalizationScoreForQuery(generalizationScoreForQuery), with_trace(withTrace), tuning_factor(tuningFactor),
          use_path_with_lambda_factor(usePathWithLambdaFactor), lambda(lambda), max_length(maxLength),
          min_prob(minProb), precision(precision) {}
