//
// Created by giacomo on 11/11/20.
//

#include "ConfigurationFile.h"


#include <fstream>
#include <rapidxml/rapidxml.hpp>
#include <regex>
#include <GenericGraph.h>



#include <utils/xml_utils.h>

std::vector<Transaction<std::string>> xesParse(const std::string& filename = "/home/giacomo/Immagini/split-miner-2.0/sepsis.xes") {

    rapidxml::xml_document<> doc;
    std::vector<char> buffer;
    rapidxml::xml_node<> *root_node = init_XML_parse(filename, "log", doc, buffer);
    std::string concept_name{"concept:name"};
    std::vector<Transaction<std::string>> log;

    SIBLING_ITERATE(trace, root_node, "trace", false) {
        Transaction<std::string>& transaction = log.emplace_back();
        SIBLING_ITERATE(event, trace, "event", false) {
            SIBLING_ITERATE(t, event, "string", false) {
                if (GET_ATTRIBUTE(t, "key") == concept_name) {
                    transaction.emplace_back(GET_ATTRIBUTE(t, "value"));
                }
            }
        }
    }

    return log;
}

/**
 * Parsing the files generate by the toolspecific StochasticPetriNet
 */
std::vector<GenericGraph<size_t>> parsePNML(const std::string& filename = "/home/giacomo/Immagini/split-miner-2.0/petri_net_30.pnml", const std::string& epsilon = ".") {
    std::regex regex{"a\\d+"};
    std::vector<char> buffer;
    std::string weight_label{"weight"};
    std::string initial_label;

    rapidxml::xml_document<> doc;
    rapidxml::xml_node<> *root_node = init_XML_parse(filename, "pnml", doc, buffer);

    std::unordered_map<std::string, size_t> label_to_id;
    std::vector<GenericGraph<size_t>> petri_nets;

    if (root_node) {
        SIBLING_ITERATE(spn, root_node, "net", false) {
            const char* spn_id = GET_ATTRIBUTE(spn, "id");
            SIBLING_ITERATE(spn_page, spn, "page", false) {
                size_t nodeId = 0;
                const char* spn_page_id = GET_ATTRIBUTE(spn_page, "id");
                GenericGraph<size_t>& graph = petri_nets.emplace_back();
                graph.setName((std::string() + spn_id) + '-' +  spn_page_id);
                graph.setWeight(1.0);
                // Having one petri net

                // Iterating over the nodes
                SIBLING_ITERATE(spn_node, spn_page, "transition", false) {
                    std::string node_id = GET_ATTRIBUTE(spn_node, "id");
                    size_t node_id_ = nodeId;
                    auto it = label_to_id.insert(std::make_pair(node_id, node_id_));
                    if (it.second) nodeId++; else node_id_ = it.first->second;

                    std::string node_label;
                    double weight = 1.0;

                    {
                        rapidxml::xml_node<> * nodeLabel = GET_FIRST_CHILD(spn_node, "name");
                        if (nodeLabel) {
                            nodeLabel = GET_FIRST_CHILD(nodeLabel, "text");
                            node_label = nodeLabel ? nodeLabel->value() : node_id;
                        } else {
                            node_label = node_id;
                        }
                    }

                    if (STRING_STARTS_WITH(node_label, "tau") ||
                        STRING_STARTS_WITH(node_label, "source ") ||
                        STRING_STARTS_WITH(node_label, "sink ") ||
                        STRING_STARTS_WITH(node_label, "newPlace") ||
                        STRING_STARTS_WITH(node_label, "middle ") ||
                        STRING_STARTS_WITH(node_label, "replacement source ") ||
                        std::regex_match(node_label, regex)) {
                        node_label = epsilon;
                    }

                    {
                        rapidxml::xml_node<> * toolSpecific = GET_FIRST_CHILD(spn_node, "toolspecific");
                        if (toolSpecific) {
                            SIBLING_ITERATE(toolAttribute, toolSpecific, "property", false) {
                                if (GET_ATTRIBUTE(toolAttribute, "key") == weight_label) {
                                    weight = std::stod(toolAttribute->value());
                                }
                            }
                        }
                    }

                    graph.add_node(node_id_, node_label, weight);
                    ///std::cout << node_id << "[l='" << node_label << "'] ~= " << weight << std::endl; // TODO!
                }

                // Retrieving the initial state
                SIBLING_ITERATE(spn_node, spn_page, "place", false) {
                    std::string node_id = GET_ATTRIBUTE(spn_node, "id");
                    rapidxml::xml_node<> * nodeLabel = GET_FIRST_CHILD(spn_node, "initialMarking");
                    if (nodeLabel) {
                        assert(initial_label.empty());
                        initial_label = node_id;
                        graph.setStart(label_to_id.at(initial_label));
                        ///std::cout << "Initial state is = " << initial_label << std::endl; // TODO!
                        break; // Stop the iteration, I've already found it!
                    }
                }

                // Retrieving the transitions
                SIBLING_ITERATE(spn_node, spn_page, "arc", false) {
                    std::string src = GET_ATTRIBUTE(spn_node, "source");
                    std::string dst = GET_ATTRIBUTE(spn_node, "target");
                    graph.add_edge(label_to_id.at(src), label_to_id.at(dst), 1.0);
                    ///std::cout << label_to_id.at(src) << "-->" << label_to_id.at(dst) << std::endl; // TODO!
                }

                graph.inferTerminalNode();
                petri_nets.emplace_back(graph);
            }
        }
    } else {
        // return empty list of PPN graphs
    }
    return petri_nets;
}


/**
 * Ingests the output of SplitMiner 2
 * @return
 */
std::vector<GenericGraph<size_t>> parseJoinMergeBPMN(const std::string& filename = "file:///home/giacomo/Immagini/split-miner-2.0/sepsis_out.xml", const std::string& epsilon = ".") {


    std::vector<char> buffer;
    rapidxml::xml_document<> doc;
    rapidxml::xml_node<> *root_node = init_XML_parse(filename, "definitions", doc, buffer);
    std::vector<GenericGraph<size_t>> petri_nets;
    if (root_node) {
        SIBLING_ITERATE(bpmn, root_node, "process", false) {
            size_t nodeId = 0;
            const char* spn_id = GET_ATTRIBUTE(bpmn, "id");
            std::string initial_label;
            std::string final_label;
            GenericGraph<size_t>& graph = petri_nets.emplace_back();
            graph.setName(spn_id);
            graph.setWeight(1.0);
            std::unordered_map<std::string, size_t> label_to_id;

            SIBLING_ITERATE(spn_node, bpmn, "task", false) {
                std::string node_id = GET_ATTRIBUTE(spn_node, "id");
                std::string node_label = GET_ATTRIBUTE(spn_node, "name");

                size_t node_id_ = nodeId;
                auto it = label_to_id.insert(std::make_pair(node_id, node_id_));
                if (it.second) nodeId++; else node_id_ = it.first->second;
                graph.add_node(node_id_, node_label, 1.0);

                if (GET_FIRST_CHILD(spn_node, "standardLoopCharacteristics")) {
                    graph.add_edge(node_id_, node_id_);
                }
            }

            SIBLING_ITERATE(spn_node, bpmn, "exclusiveGateway", false) {
                std::string node_id = GET_ATTRIBUTE(spn_node, "id");
                std::string node_label = epsilon;

                size_t node_id_ = nodeId;
                auto it = label_to_id.insert(std::make_pair(node_id, node_id_));
                if (it.second) nodeId++; else node_id_ = it.first->second;
                graph.add_node(node_id_, node_label, 1.0);
            }

            {
                rapidxml::xml_node<> * start =  GET_FIRST_CHILD(bpmn, "startEvent");
                assert(start);
                initial_label = GET_ATTRIBUTE(start, "id");
                graph.setStart(label_to_id.at(initial_label));
            }
            {
                rapidxml::xml_node<> * end =  GET_FIRST_CHILD(bpmn, "endEvent");
                assert(end);
                final_label = GET_ATTRIBUTE(end, "id");
                graph.setEnd(label_to_id.at(final_label));
            }

            SIBLING_ITERATE(spn_node, bpmn, "sequenceFlow", false) {
                std::string src = GET_ATTRIBUTE(spn_node, "sourceRef");
                std::string dst = GET_ATTRIBUTE(spn_node, "targetRef");
                graph.add_edge(label_to_id.at(src), label_to_id.at(dst));
            }
        }
    }
    return petri_nets;
}

extern "C" {
#include <stdio.h>
}
#ifndef EOF
#define EOF (-1)
#endif

#include <GenericGraph.h>
#include <distances/WeightEstimator.h>

GenericGraph<size_t> parsePetriMatrix(const std::string& filename = "data/petri_matrix.txt", bool isFile = true) {
    GenericGraph<size_t> graph;
    FILE *file;
    size_t source = 0, target = 0;
    if (isFile)
        file = fopen(filename.c_str(), "r");
    else
        file = fmemopen((void*)filename.c_str(), filename.size(), "r");
    size_t error = 0;
    size_t nodes = 0, edges = 0;
    if (file) {
        int i;
        double w = 1;
        // Reading the number of the nodes
        i = fscanf(file, "nodes: %zd\n", &nodes);
        error = (i == EOF || (i != 1));
        if (error) return graph;
        i = fscanf(file, "edges: %zd\n", &edges);
        error = (i == EOF || (i != 1));
        if (error) return graph;
        i = fscanf(file, "source: %zd\n", &source);
        error = (i == EOF || (i != 1));
        if (error) return graph;
        graph.setStart(source);
        i = fscanf(file, "target: %zd\n", &target);
        error = (i == EOF || (i != 1));
        if (error) return graph;
        graph.setEnd(target);
        i = fscanf(file, "weight: %lf\n", &w);
        error = (i == EOF || (i != 1));
        if (error) return graph;
        graph.setWeight(w);

        for (size_t j = 0; j<nodes; j++) {
            size_t node_no;
            char string[124];
            std::string k;
            double weight;
            i = fscanf(file, "%zd %lf %123s\n", &node_no, &weight, string);
            error = (i == EOF || (i != 3));
            if (error) return graph;
            k = std::string(string);
            graph.add_node(node_no, string, weight);
        }


        for (size_t j = 0; j<edges; j++) {
            size_t src, dst;
            double weight;
            i = fscanf(file, "%zd %zd\n", &src, &dst);
            error = (i == EOF || (i != 2));
            if (error) return graph;
            graph.add_edge(src, dst);
        }
        fclose(file);
    }

    return graph;
}

GenericGraph<size_t> stochasticMatrix(const std::string& filename) {
    GenericGraph<size_t> graph;
    FILE* file = fopen(filename.c_str(), "r");
    size_t error = 0;
    size_t nodes = 0, edges = 0;
    size_t source = 0, target = 0;
    if (file) {
        int i;
        double w = 1;
        // Reading the number of the nodes
        i = fscanf(file, "nodes: %zd\n", &nodes);
        error = (i == EOF || (i != 1));
        if (error) return graph;
        i = fscanf(file, "edges: %zd\n", &edges);
        error = (i == EOF || (i != 1));
        if (error) return graph;
        i = fscanf(file, "source: %zd\n", &source);
        error = (i == EOF || (i != 1));
        if (error) return graph;
        graph.setStart(source);
        i = fscanf(file, "target: %zd\n", &target);
        error = (i == EOF || (i != 1));
        if (error) return graph;
        graph.setEnd(target);
        i = fscanf(file, "weight: %lf\n", &w);
        error = (i == EOF || (i != 1));
        if (error) return graph;
        graph.setWeight(w);

        for (size_t j = 0; j<nodes; j++) {
            size_t node_no;
            char string[124];
            std::string k;
            i = fscanf(file, "%zd %123s\n", &node_no, string);
            error = (i == EOF || (i != 2));
            if (error) return graph;
            k = std::string(string);
            graph.add_node(node_no, k);
        }


        for (size_t j = 0; j<edges; j++) {
            size_t src, dst;
            double weight;
            i = fscanf(file, "%zd %zd %lf\n", &src, &dst, &weight);
            error = (i == EOF || (i != 3));
            if (error) return graph;
            assert(std::abs(weight) <= 1.0);
            graph.add_edge(src, dst, weight);
        }
        fclose(file);
    }
    return graph;
}


bool isFileFormatPetri(enum FileFormat format) {
    switch (format) {

        case Petri_PNML:
        case Petri_BPMN:
        case PetriMatrix:
            return true;

        case StochasticMatrix:
        case ProbRegex:
        default:
            return false;
    }
}

#include <iostream>
#include <random>
#define READ_GRAPH(format)      std::cout << "Reading graph '" << this->input_file << "' as a " << format << " file. Getting the " << this->ith_graph << "-th graph" << std::endl

void performLogOperation(const std::vector<LogOperationConfiguration>& vconf,
                         std::vector<Transaction<std::string>>& log) {

    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<double> range(0.0,1.0);

    for (const auto& conf : vconf) {
        switch (conf.operation) {
            case LogSample: {
                std::cout << "LogSample: shuffling. Keep Low = " << conf.keep_low_up_otherwise << std::endl;
                std::shuffle(log.begin(), log.end(), g);
                auto first = log.begin() + (conf.keep_low_up_otherwise ? 0 : ((size_t)conf.factor*log.size()) );
                auto last = log.begin() + (conf.keep_low_up_otherwise ? ((size_t)conf.factor*log.size()) : (log.size()-1) );
                std::vector<Transaction<std::string>> newVec(first, last);
                std::cout << "Reduced size, from " << log.size() << " to " << newVec.size() << std::endl;
                log = newVec;
            }
            break;

            case LogFilterFrequency: {
                std::vector<Transaction<std::string>> output;
                std::map<double, std::vector<size_t>> ordering;
                double N = log.size();
                std::cout << "LogSample: histogramming. Keep Low = " << conf.keep_low_up_otherwise << std::endl;
                for (size_t i = 0, n = log.size(); i<n; i++) {
                    ordering[log[i].size()].emplace_back(i);
                }
                if (conf.keep_low_up_otherwise) {
                    for (auto it = ordering.begin(), en = ordering.end(); it != en; it++) {
                        for (const auto& i : it->second) {
                            output.emplace_back(log[i]);
                        }
                        if (conf.factor <= ((double)ordering.size())/N) break;
                    }
                } else {
                    for (auto it = ordering.rbegin(), en = ordering.rend(); it != en; it++) {
                        for (const auto& i : it->second) {
                            output.emplace_back(log[i]);
                        }
                        if (conf.factor <= ((double)ordering.size())/N) break;
                    }
                }
                std::cout << "Reduced size, from " << log.size() << " to " << output.size() << std::endl;
                log = output;
            }
            break;

            case LogFilterValues: {
                double max = -1.0;
                double min = std::numeric_limits<double>::max();
                for (const auto& t : log) {
                    double x = t.size();
                    max = std::max(max, x);
                    min = std::min(min, x);
                }
                double value = conf.factor * (max - min) + min;
                std::cout << "Log max = " << max << " Log min = " << min << " LogFilterValue = " << conf.factor << " Limit value = " << value << " Keep Low = " << conf.keep_low_up_otherwise << std::endl;
                std::vector<Transaction<std::string>> output;
                for (const auto& t : log) {
                    if (conf.keep_low_up_otherwise) {
                        if (((double )t.size()) <= value) output.emplace_back(t);
                    } else {
                        if (((double )t.size()) >= value) output.emplace_back(t);
                    }
                }
                std::cout << "Reduced size, from " << log.size() << " to " << output.size() << std::endl;
                log = output;
            }
            break;
        }
    }


}


#include "Regex.h"

std::pair<size_t, size_t>
generateForLoops(GenericGraph<size_t>& graph, const std::pair<size_t, size_t> &result, bool doSkip,
                 bool doLoop, double cost, size_t& nNodes, const std::string& epsilon = ".") {
    assert(std::abs(cost) <= 1.0);
    size_t left = nNodes;
    graph.add_node(left, epsilon);
    nNodes++;

    size_t right = nNodes;
    graph.add_node(right, epsilon);
    nNodes++;
    /*auto left  = addEpsilonNode();
    auto right = addEpsilonNode();*/
    double rightCost = 1.0;
    double leftCost  = 1.0;

    if (doLoop) {
        graph.add_edge(result.second, result.first, cost);
        rightCost = 1.0-cost;
    }

    if (doSkip) {
        graph.add_edge(left, right, cost);
        leftCost = 1.0-cost;
    }

    graph.add_edge(left, result.first, leftCost);
    graph.add_edge(result.second, right, rightCost);
    return {left, right};
}

std::pair<size_t, size_t> generate(GenericGraph<size_t>& graph, Regex<std::string> *element, size_t& nNodes, const std::string& epsilon = ".") {
    assert(element);
    switch (element->cases()) {
        case Consequent:
        {
            auto left = generate(graph, element->getLeft(), nNodes, epsilon);
            auto right = generate(graph, element->getRight(), nNodes, epsilon);
            graph.add_edge(left.second, right.first, element->getCost());
            return {left.first, right.second};
        }

        case BinaryAlternative:
        {
            const double cost = element->getCost();
            assert(std::abs(cost) <= 1.0);
            auto left = generate(graph, element->getLeft(), nNodes, epsilon);
            auto right = generate(graph, element->getRight(), nNodes, epsilon);

            auto src =  nNodes;
            graph.add_node(src, epsilon);
            nNodes++;
            graph.add_edge(src, left.first, cost);
            graph.add_edge(src, right.first, 1.0-cost);

            auto dst =  nNodes;
            graph.add_node(dst, epsilon);
            nNodes++;
            graph.add_edge(left.second, dst, 1.0);
            graph.add_edge(right.second, dst, 1.0);

            return {src, dst};
        }
            break;
        case Plus:
            return generateForLoops(graph, generate(graph, element->getLeft(), nNodes, epsilon), false, true, element->getCost(), nNodes, epsilon);
        case Star:
            return generateForLoops(graph, generate(graph, element->getLeft(), nNodes, epsilon), true, true, element->getCost(), nNodes, epsilon);
        case Optional:
            return generateForLoops(graph, generate(graph, element->getLeft(), nNodes, epsilon), true, false, element->getCost(), nNodes, epsilon);
        case Paren:
            return generate(graph, element->getLeft(), nNodes, epsilon);
        case String: {
            size_t id = nNodes;
            graph.add_node(id, element->getLabel());
            nNodes++;
            return {id, id};
        }
        case NONE:
            assert(false);
    }
}


#include <ANTLRInputStream.h>
#include <fstream>
#include <regexLexer.h>
#include <regexParser.h>
#include <RegexVisitor.h>

GenericGraph<size_t> parseREGEX(const std::string& filename, const std::string& epsilon) {

    const char empty[0] = {};
    antlr4::ANTLRInputStream stream(empty, 0);
    std::ifstream file(filename, std::ifstream::binary);
    stream.load(file);
    regexLexer lexer{&stream};
    //regexLexer* lexer = new regexLexer(&stream);
    // Generate the tokens.
    antlr4::CommonTokenStream tokens(&lexer);

    file.close();

    tokens.fill();

    // Create the translation that will parse the input.
    regexParser parser(&tokens);
    parser.setBuildParseTree(true);
    RegexVisitor visitor;
    Regex<std::string>* regex = visitor.visit(parser.regex()).as<Regex<std::string>*>();
    GenericGraph<size_t> graph;
    graph.setName(filename);
    size_t nNodes = 0;
    auto it = generate(graph, regex, nNodes, epsilon);
    graph.setStart(it.first);
    graph.setEnd(it.second);
    graph.setWeight(1.0);
    return graph;
}
void ConfigurationFile::load() {
    GenericGraph<size_t> graph;
    spd_we::WeightEstimator<size_t> we;
    std::vector<Transaction<std::string>> log;
    admissibleCharList.erase(std::remove(admissibleCharList.begin(), admissibleCharList.end(), varepsilon), admissibleCharList.end());
    std::string epsilon{this->varepsilon};

    switch (this->input_file_format) {
        case Petri_PNML:
            READ_GRAPH("PNML (Stochastic Petri Net)");
            graph = parsePNML(this->input_file, epsilon)[this->ith_graph];
            break;

        case Petri_BPMN:
            READ_GRAPH("Simple BPMN (to Stochastic Petri Net)");
            graph = parseJoinMergeBPMN(this->input_file, epsilon)[this->ith_graph];
            break;

        case PetriMatrix:
            READ_GRAPH("weighted- and labelled- node matrix");
            graph = parsePetriMatrix(this->input_file);
            break;

        case StochasticMatrix: // TODO
            READ_GRAPH("node-labelled and edge-weigthed (stochastic) matrix");
            graph = stochasticMatrix(this->input_file);
            break;

        case ProbRegex: // TODO
            READ_GRAPH("probabilistic REGEX");
            graph = parseREGEX(this->input_file, epsilon);
            break;
    }

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
            }
            we.setGraph(&graph);

            switch (this->trace_file_format) {
                case XESLog:
                    log = xesParse(this->traces_file);
                    performLogOperation(this->operations, log);
                    we.setLog(log);
                    break;
                case RawLog:
                    log = read_log(this->traces_file, this->separator_if_any);
                    performLogOperation(this->operations, log);
                    we.setLog(log);
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
        }
    }

    std::cout << "Performing the varepsilon-closure... " << std::flush;
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        graph.doClosure(epsilon);
        auto now = std::chrono::high_resolution_clock::now();
        std::cout << " done (" << std::chrono::duration_cast<std::chrono::nanoseconds>(now-t1).count() << " ns)" << std::endl;
    }

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
    for (const auto& trace : log) {
        std::string stringBuilder;
        for (const auto& str : trace) {
            stringBuilder += action_to_single_char.getValue(str);
        }
        finalLog.emplace(stringBuilder);
    }

    finalGraph.printGraph();

    if (this->add_traces_to_log) {
        std::cout << "Adding some further traces to the log from the generated paths. Settings: maxLength = " << this->max_length << " minProb = " << this->min_prob << std::endl;
        for (const auto& path : finalGraph.iterateOverPaths(false, max_length, min_prob)) {
            std::cout << " New trace = '" << path.path << "' with probability = " << path.cost << std::endl;
            finalLog.emplace(path.path);
        }
    }

}
