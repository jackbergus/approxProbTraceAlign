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
    std::vector<Transaction<std::string>> logFromFile;
    admissibleCharList.erase(std::remove(admissibleCharList.begin(), admissibleCharList.end(), varepsilon), admissibleCharList.end());
    std::string epsilon{this->varepsilon};

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
                    performLogOperation(this->operations, logFromFile);
                    rememberToLog = true;
                    break;
                case RawLog:
                    logFromFile = read_log(this->traces_file, this->separator_if_any);
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
    for (const auto& trace : logFromFile) {
        std::string stringBuilder;
        for (const auto& str : trace) {
            stringBuilder += action_to_single_char.getValue(str);
        }
        finalLog.emplace_back(1.0, stringBuilder, std::vector<size_t>{});
    }


    if (this->add_traces_to_log) {
        std::cout << "5) Adding some further traces to the log from the generated paths. Settings: maxLength = " << this->max_length << " minProb = " << this->min_prob << std::endl;
        for (const auto& path : finalGraph.iterateOverPaths(false, max_length, min_prob)) {
            std::cout << " New trace = '" << path.path << "' with probability = " << path.probability << std::endl;
            finalLog.emplace_back(path);
        }
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
    out << DBL_SERIALIZE(noiseThreshold);
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
        PARSE_DBL(min_prob);

        PARSE_BOOL(use_estimator);
        PARSE_ENUM(estimator_type, spd_we::WeightEstimatorCases);

        PARSE_STRING(admissibleCharList);
        PARSE_DBL(noiseThreshold);
        PARSE_CHAR(varepsilon);
        PARSE_INT(seedError);

        traceNoiser = AlterString{admissibleCharList, noiseThreshold, seedError}; // Setting the trace noiser from the default arguments

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
