//
// Created by giacomo on 11/11/20.
//

#include "ConfigurationFile.h"
#include "benchmarking/new/LogBenchmark.h"
#include "benchmarking/new/MultithreadedBenchmarkForPooling.h"
#include "../AbstractKNN.h"
#include <regex>
#include <GenericGraph.h>
#include <distances/WeightEstimator.h>
#include <log/log_operations.h>
#include <iostream>
#include <topk/Aussageform/DistanceExpressionEvaluator.h> //--> DO NOT INCLUDE IN THE .H! For this reason, I opted out

#define READ_GRAPH(format)      std::cout << "Reading graph '" << this->input_file << "' as a " << format << " file. Getting the " << this->ith_graph << "-th graph" << std::endl

#include <magic_enum.hpp>
#include <topk/old/minimum_edit_maximum_substring.h>
#include <distances/strings/LevensteinSimilarity.h>
#include <thread>


double ConfigurationFile::set_of_path_similarity(const std::vector<path_info>& tracesSet, const std::vector<path_info>& querySet, const LevensteinSimilarity& sim, std::vector<additional_benchmarks_per_log>* opt, double* precisionNormalization, PathEmbeddingStrategy* strategy) {
    double cost = 0.0;
    for (const auto& x : tracesSet) {
        for (const auto& y : querySet) {
            double val = sim.similarity(x.path, y.path)*(x.probability*y.probability);
            /*if (opt && precisionNormalization && strategy) {
                opt->emplace_back(input_file, traces_file, x.path, x.path.size(), *strategy, 0.0,  false, tuning_factor, use_path_lambda_factor, lambda, max_length, min_prob, LogPrecisionMetric, val/(*precisionNormalization), y.path, y.path.size());
            }*/
            cost += val;
        }
    }
    return cost;
}


#include <chrono>
using namespace std::chrono;

struct VpTreeStructDistance {
    virtual double operator()(const path_info& lhs, const path_info& rhs) const {
        return lhs.probability*rhs.probability*GeneralizedLevensteinDistance(lhs.path, rhs.path);
    }
};

struct VpTreePairPow2Distance {
    virtual double operator()(const std::pair<double, double>& lhs, const std::pair<double,double>& rhs) const {
        return (lhs.first-rhs.first)*(lhs.first-rhs.first)+(lhs.second-rhs.second)*(lhs.second-rhs.second);
    }
};

struct VpTreePairPow2DotProduct {
    virtual double operator()(const Eigen::VectorXd& lhs, const Eigen::VectorXd& rhs) const {
        return 1.0-lhs.dot(rhs)/std::sqrt(lhs.dot(lhs)*rhs.dot(rhs));
    }
};


/** Euclidean distance functor.
  * This the same as the L2 minkowski distance but more efficient.
  * @see ManhattenDistance, ChebyshevDistance, MinkowskiDistance */
template <typename Scalar>
struct DotProductDistance
{
    /** Compute the unrooted distance between two vectors.
      * @param lhs vector on left hand side
      * @param rhs vector on right hand side */
    template<typename DerivedA, typename DerivedB>
    Scalar operator()(const Eigen::MatrixBase<DerivedA> &lhs,
                      const Eigen::MatrixBase<DerivedB> &rhs) const
    {
        static_assert(
                std::is_same<typename Eigen::MatrixBase<DerivedA>::Scalar,Scalar>::value,
                "distance scalar and input matrix A must have same type");
        static_assert(
                std::is_same<typename Eigen::MatrixBase<DerivedB>::Scalar, Scalar>::value,
                "distance scalar and input matrix B must have same type");

        return 1.0-lhs.dot(rhs)/std::sqrt(lhs.dot(lhs)*rhs.dot(rhs));
    }

    /** Compute the unrooted distance between two scalars.
      * @param lhs scalar on left hand side
      * @param rhs scalar on right hand side */
    Scalar operator()(const Scalar lhs,
                      const Scalar rhs) const
    {
        Scalar diff = lhs - rhs;
        return diff * diff;
    }

    /** Compute the root of a unrooted distance value.
      * @param value unrooted distance value */
    Scalar operator()(const Scalar val) const
    {
        return std::sqrt(val);
    }
};

#include <multithreaded/MultithreadWrap.h>
#include <data_structures/vptree.h>
#include <knn/multi_index_hashing.h>
typedef Eigen::MatrixXd Matrix;
typedef knn::Matrixi Matrixi;


void ConfigurationFile::run() {

    {
        std::ostringstream  oss;
        std::time_t t = std::time(nullptr);
        std::tm tm = *std::localtime(&t);
        oss << std::put_time(&tm, "%F %T%z - %a %e %b, %Y") << std::endl;
        {
            std::string str = oss.str();
            str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
            this->results_folder = configuration_filename.substr(0, configuration_filename.find_last_of("."));
        }
        std::filesystem::create_directory(results_folder);
        serialize((results_folder / "configuration.yaml").c_str());
    }

    GenericGraph<size_t> graph;
    spd_we::WeightEstimator<size_t> we;
    we.setVarEpsilon({varepsilon});
    std::vector<Transaction<TimestampedEvent>> logForWeightEstimation, originalLog, logForBenchmarks;
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
    graph.removeSolitaryNodes();
    graph.render((this->results_folder / "graph_01_input.pdf").c_str());
    bool rememberToLog = false;
    bool useEstimator = false;


    if (isFileFormatPetri(this->input_file_format)) {
        /*if (!this->use_estimator) {
            std::cout << "Using no estimator: no traces are going to be loaded!" << std::endl;
            if (this->trace_file_format != NoLog) {
                std::cerr << "Ignoring the log file: no estimator is used" << std::endl;
                this->trace_file_format = NoLog;
            }
        } else*/ {
            if (this->estimator_type == spd_we::W_CONSTANT) {
                std::cout << "Using the constant estimator" << std::endl;
                ///std::cerr << "Ignoring the log file: the constant estimator will be used" << std::endl;
                ///this->trace_file_format = NoLog;
            } else {
                std::cout << "Using an estimator: a log file will be loaded" << std::endl;
                useEstimator = true;
            }

            switch (this->trace_file_format) {
                case XESLog:
                    logForWeightEstimation = load_xes(this->traces_file);
                    originalLog = logForWeightEstimation;
                    performLogOperation(this->operations, logForWeightEstimation);
                    {
                        std::set<std::vector<TimestampedEvent>> difference;
                        for (const auto& x : originalLog) {
                            difference.emplace(x);
                        }
                        for (const auto& x : logForWeightEstimation) {
                            difference.erase(difference.find(x));
                        }
                        for (const auto& x : difference) {
                            logForBenchmarks.emplace_back(x);
                        }
                    }
                    rememberToLog = true;
                    break;
                case RawLog:
                    logForWeightEstimation = read_log(this->traces_file, this->separator_if_any);
                    originalLog = logForWeightEstimation;
                    performLogOperation(this->operations, logForWeightEstimation);
                    {
                        std::set<std::vector<TimestampedEvent>> difference;
                        for (const auto& x : originalLog) {
                            difference.emplace(x);
                        }
                        for (const auto& x : logForWeightEstimation) {
                            difference.erase(difference.find(x));
                        }
                        for (const auto& x : difference) {
                            logForBenchmarks.emplace_back(x);
                        }
                    }
                    rememberToLog = true;
                    break;

                case NoLog:
                    std::cerr << "No trace file is provided: either the a constant weight or the pre-loaded weight will be set"  << std::endl;
                    break;
            }
        }


        std::cout << "Trasforming the PetriNet (IS) into a Thompson automaton [transferring the edges from the nodes to the transitions]... " << std::flush;
        {
            auto t1 = std::chrono::steady_clock::now();
            graph.transfer_weight_from_nodes_to_edges();
            auto now = std::chrono::steady_clock::now();
            std::cout << " done (" << std::chrono::duration_cast<std::chrono::nanoseconds>(now-t1).count() << " ns)" << std::endl;
            graph.render((this->results_folder / "graph_02_weight_transfer.pdf").c_str());
        }
    }

    std::cout << "Performing the varepsilon-closure... " << std::flush;
    {
        auto t1 = std::chrono::steady_clock::now();
        graph.doClosure(epsilon);
        auto now = std::chrono::steady_clock::now();
        std::cout << " done (" << std::chrono::duration_cast<std::chrono::nanoseconds>(now-t1).count() << " ns)" << std::endl;
        graph.render((this->results_folder / "graph_03_e-closed.pdf").c_str());
    }


    if (rememberToLog && useEstimator) {
        auto t1 = std::chrono::steady_clock::now();
        std::cout << "Retrieving the estimator phase after the varepsilon closure... " << std::flush;
        we.setGraph(&graph);
        we.setLog(logForWeightEstimation);
        for (const auto& id : graph.getNodes()) {
            graph.updateNodeWeight(id, we.getNodeWeight(id, this->estimator_type));
        }
        graph.render((this->results_folder / "graph_04_estimator.pdf").c_str());
        graph.transfer_weight_from_nodes_to_edges();
        graph.render((this->results_folder / "graph_04_estimator_transfer.pdf").c_str());
        auto now = std::chrono::steady_clock::now();
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
    {
        std::string newAdmissibleCharList; // Avoiding to insert some chars that are not within the trace
        for (const auto& cp : action_to_single_char.getElements()) {
            newAdmissibleCharList += cp.second;
            std::cout<< "  - " << cp.first << " <-> " << cp.second << std::endl;
        }
        std::swap(admissibleCharList, newAdmissibleCharList);
    }
    traceNoiser.clear();
    for (const auto& dbl : noiseThreshold) {
        traceNoiser.emplace_back(admissibleCharList, dbl, seedError);
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
    convertLog(logForBenchmarks, finalLog);
    convertLog(originalLog, finalOriginalLog);

    if (this->add_traces_to_log) {
        std::cout << "5) Adding some further traces to the log from the generated paths. Settings: maxLength = " << this->max_length << " minProb = " << this->min_prob << std::endl;
        for (const auto& path : finalGraph.iterateOverPaths(false, max_length, min_prob)) {
            std::cout << " New trace = '" << path.path << "' with probability = " << path.probability << std::endl;
            finalLog.emplace_back(path);
        }
    }

    std::cout << "6) Sampling some traces, just for limiting the execution time (for the moment)" << std::endl;
    size_t N = 3;
    {
        std::mt19937 sd(std::default_random_engine{}());
        std::map<size_t, std::unordered_set<struct path_info>> selectionMap; // Selecting some traces, while removing the duplicate traces
        for (const struct path_info& v : finalLog) {
            selectionMap[v.path.size()].emplace(v);
        }
        finalLog.clear();
        for (auto& cp : selectionMap) {
            std::vector<struct path_info> sampled;
            std::sample(cp.second.begin(), cp.second.end(), std::back_inserter(finalLog), N, sd);
        }
        /*max_length = selectionMap.rbegin()->first;
        std::cout << "The maximum value has been changed to " << max_length;*/
    }

    constexpr auto& color_entries =  magic_enum::enum_entries<PathEmbeddingStrategy>();
    constexpr std::size_t color_count = magic_enum::enum_count<PathEmbeddingStrategy>();
    ///DistanceExpressionEvaluator *localMetric = fileStrategyMap_loaded[UnterstuetzenStrategie::ProbabilitySimilarity];
    bool isMultiThreaded = true;

    /*MultithreadWrap<std::pair<std::vector<record_element_per_query>, std::vector<additional_benchmarks_per_log>>> pool{(unsigned int)std::thread::hardware_concurrency(), isMultiThreaded};
    std::vector<MultithreadedBenchmarkForPooling> toMultiThread;
    size_t id = 0;*/
    std::vector<std::pair<std::string, std::vector<std::pair<std::string,double>>>> V;
    for (const auto& query : finalLog) {
        std::pair<std::string, std::vector<std::pair<std::string,double>>> cp{query.path, {}};
        for (auto& noiser : traceNoiser) {
            std::string alteredQuery;
            do {
                alteredQuery = noiser.alter(query.path);
            } while ((alteredQuery == query.path) || (alteredQuery.empty()));
            cp.second.emplace_back(alteredQuery, noiser.noiseThreshold);
        }
        V.emplace_back(cp);
    }
    std::cout << std::endl << "V map loaded" << std::endl;

    std::string logFile1 = (results_folder / "log_proposed_main.csv").string();
    std::ofstream log1(logFile1, std::ofstream::out);
    std::string logFile2= (results_folder / "log_proposed_time.csv").string();
    std::ofstream log2(logFile2, std::ofstream::out);

    for (size_t i = 0; i<color_count; i++) {
        std::set<std::pair<std::string,std::string>> embedding_space;
        std::cout << "Strategy " << (i+1) << " of " << color_count << ": " << color_entries[i].second.data() << std::endl;
        PathEmbeddingStrategy strategy = color_entries[i].first;
        std::string strategyName{magic_enum::enum_name(strategy).data()};

        MultiplePathsEmbeddingStrategy* pathstrategy  = generatePathEmbeddingStrategyFromParameters(strategy);
        GraphEmbeddingStrategy *        graphStrategy = generateGraphEmbeddingStrategyFromParameters(strategy);


        std::cout << " * generating embeddings... " << std::flush;
        ReadGraph::path_to_uembedding ptg;
        ReadGraph::path_to_uembedding ptu = (*pathstrategy)(finalGraph);
        ReadGraph::extractEmbeddingSpace(embedding_space, ptu);
        auto map = ReadGraph::generateStructuredEmbeddings(embedding_space, ptu);
        std::vector<struct path_info> mapPath;
        std::vector<Eigen::VectorXd> mapVecs;
        for (const auto& path : map) {
            std::cout << " ~~~~ " << path.first << std::endl;
            mapPath.emplace_back(path.first);
            mapVecs.emplace_back(path.second);
        }
        std::cout << " done" << std::endl;

        steady_clock::time_point vpTreeActualDistanceStartLoad = steady_clock::now();
        vp_tree<struct path_info, VpTreeStructDistance> vptreeActualMetric{mapPath};
        steady_clock::time_point vpTreeActualDistanceEndLoad = steady_clock::now();
        double actualMetricLoading = duration_cast<std::chrono::nanoseconds>(vpTreeActualDistanceEndLoad - vpTreeActualDistanceStartLoad).count()/1000000.0;
        std::cout << "VPTree loading time with to-be-recomputed metric: (ns) " << actualMetricLoading << std::endl;
        log2 << input_file.substr(input_file.find_last_of("/\\") + 1) << ','
             << traces_file.substr(traces_file.find_last_of("/\\") + 1)<< ','
             <<  ','
             <<  ','
             << strategyName << ','
             << 0.0 << ','
             << "VPTreeLoading+Metric," << actualMetricLoading << ",,0" << std::endl;

        steady_clock::time_point vpTreeProposedStartLoad = steady_clock::now();
        vp_tree<Eigen::VectorXd, VpTreePairPow2DotProduct> vpTreeProposed{mapVecs};
        steady_clock::time_point vpTreeProposedEndLoad = steady_clock::now();
        double proposedLoading = duration_cast<std::chrono::nanoseconds>(vpTreeProposedEndLoad - vpTreeProposedStartLoad).count()/1000000.0;
        std::cout << "VPTree loading time with to-be-recomputed metric: (ns) " << proposedLoading << std::endl;
        log2 << input_file.substr(input_file.find_last_of("/\\") + 1) << ','
             << traces_file.substr(traces_file.find_last_of("/\\") + 1)<< ','
             <<   ','
             <<   ','
             << strategyName << ','
             << 0.0 << ','
             << "VPTreeLoading+Embedding," << proposedLoading << ",,0" << std::endl;


        steady_clock::time_point knnProposedStartLoad = steady_clock::now();
        Matrix dataPoints(embedding_space.size(), map.size());
        size_t j = 0;
        for (const auto& path : map) {
            dataPoints.col(j) = path.second;
            j++;
        }
        knn::KDTreeMinkowski<double, knn::EuclideanDistance<double>> kdtreeProposed(dataPoints);
        kdtreeProposed.setCompact(true);
        kdtreeProposed.setBalanced(true);
        kdtreeProposed.setSorted(true);
        kdtreeProposed.setTakeRoot(true);
        kdtreeProposed.setMaxDistance(2.5);
        kdtreeProposed.setThreads(1);
        kdtreeProposed.build();
        steady_clock::time_point knnProposedEndLoad = steady_clock::now();
        double proposedLoadingKNN = duration_cast<std::chrono::nanoseconds>(knnProposedEndLoad - knnProposedStartLoad).count()/1000000.0;
        std::cout << "VPTree loading time with to-be-recomputed metric: (ns) " << proposedLoadingKNN << std::endl;
        log2 << input_file.substr(input_file.find_last_of("/\\") + 1) << ','
             << traces_file.substr(traces_file.find_last_of("/\\") + 1)<< ','
             <<   ','
             <<   ','
             << strategyName << ','
             << 0.0 << ','
             << "VPKDTreeMinkowski+Embedding," << proposedLoadingKNN << ",,0" << std::endl;


        std::cout << " * generation precision metric" << std::flush;
        double precision_normalization = std::sqrt(set_of_path_similarity(mapPath, mapPath, similarity)*set_of_path_similarity(finalOriginalLog, finalOriginalLog, similarity));
        double precision = set_of_path_similarity(mapPath, finalOriginalLog, similarity, &log_quality, &precision_normalization)/precision_normalization;
        std::cout << "... done" << std::endl;

        log1 << input_file.substr(input_file.find_last_of("/\\") + 1) << ','
                    << traces_file.substr(traces_file.find_last_of("/\\") + 1) << ",,0,"
                    << color_entries[i].second.data()
                    << ",0.0,F,"
                    << tuning_factor << ','
                    << (use_path_lambda_factor ? "T," : "F,")
                    << lambda << ','
                    << max_length << ','
                    << min_prob
                    << ",Pecision," << precision << ",,0" << std::endl;

        std::cout << " * starting with query analysis!" << std::endl;
        for (auto& query : V) {
            std::string actualQuery = query.first;
            {
                struct path_info forQuery{1.0, actualQuery, {}};
                steady_clock::time_point vpTreeActualDistanceStartQuery = steady_clock::now();
                vptreeActualMetric.topkSearch(forQuery, 20);
                steady_clock::time_point vpTreeActualDistanceEndQuery = steady_clock::now();
                double actualMetricQuery = duration_cast<std::chrono::nanoseconds>(vpTreeActualDistanceEndLoad - vpTreeActualDistanceStartLoad).count()/1000000.0;
                log2 << input_file.substr(input_file.find_last_of("/\\") + 1) << ','
                     << traces_file.substr(traces_file.find_last_of("/\\") + 1) << ','
                        << query.first << ','
                        << query.first.size() << ','
                        << strategyName << ','
                        << 0.0 << ','
                     << "VPTree+Metric," <<  ( actualMetricQuery) << ",,0" << std::endl;
            }

            {
                // Transformed space
                std::vector<std::pair<double,double>> transformedSpace;
                steady_clock::time_point vpTreeTransformedStartQuery = steady_clock::now();
                for (const auto& path : map) {
                    double similarity = 1.0/((GeneralizedLevensteinDistance(actualQuery, path.first.path)/5.0+1.0));
                    double probability = path.first.probability;
                    double sqrt = std::sqrt(similarity*similarity+probability*probability);
                    transformedSpace.emplace_back((1.0/(similarity*sqrt)), (1.0/(probability*sqrt)));
                }
                vp_tree<std::pair<double,double>, VpTreePairPow2Distance> transformed_tree{transformedSpace};
                transformed_tree.topkSearch(std::make_pair(0,0), 10);
                steady_clock::time_point vpTreeTransformedEndQuery = steady_clock::now();
                double transformedQuery = duration_cast<std::chrono::nanoseconds>(vpTreeTransformedEndQuery - vpTreeTransformedStartQuery).count()/1000000.0;
                log2 << input_file.substr(input_file.find_last_of("/\\") + 1) << ','
                     << traces_file.substr(traces_file.find_last_of("/\\") + 1) << ','
                     << query.first << ','
                     << query.first.size() << ','
                     << strategyName << ','
                     << 0.0 << ','
                     << "VPTree+Transformed," << transformedQuery << ",,0" << std::endl;
            }

            {
                steady_clock::time_point vpTreeTransformedStartQuery = steady_clock::now();
                Matrix dataPoints(2, map.size());
                size_t j = 0;
                for (const auto& path : map) {
                    double similarity = 1.0/((GeneralizedLevensteinDistance(actualQuery, path.first.path)/5.0+1.0));
                    double probability = path.first.probability;
                    double sqrt = std::sqrt(similarity*similarity+probability*probability);
                    dataPoints(0,j) = (1.0/(similarity*sqrt));
                    dataPoints(1,j) = (1.0/(probability*sqrt));
                    j++;
                }
                knn::KDTreeMinkowski<double, knn::EuclideanDistance<double>> kdtree(dataPoints);
                kdtree.setCompact(true);
                kdtree.setBalanced(true);
                kdtree.setSorted(true);
                kdtree.setTakeRoot(true);
                kdtree.setMaxDistance(2.5);
                kdtree.setThreads(1);
                kdtree.build();
                Matrix queryPoints(2, 1);
                queryPoints << 0,  0;
                Matrixi indices;
                Matrix distances;
                kdtree.query(queryPoints, 10, indices, distances);
                steady_clock::time_point vpTreeTransformedEndQuery = steady_clock::now();
                double transformedQuery = duration_cast<std::chrono::nanoseconds>(vpTreeTransformedEndQuery - vpTreeTransformedStartQuery).count()/1000000.0;
                log2 << input_file.substr(input_file.find_last_of("/\\") + 1) << ','
                     << traces_file.substr(traces_file.find_last_of("/\\") + 1) << ','
                     << query.first << ','
                     << query.first.size() << ','
                     << strategyName << ','
                     << 0.0 << ','
                     << "VPKDTreeMinkowski+Transformed," << transformedQuery << ",,0" << std::endl;
            }


            {
                ReadGraph g = ReadGraph::fromString(actualQuery, 1.0);
                auto tmp = (*graphStrategy)(g);
                auto x = ReadGraph::generateStructuredEmbedding(embedding_space, tmp);


                steady_clock::time_point vpTreeTransformedStartQuery = steady_clock::now();
                vpTreeProposed.topkSearch(x, 10);
                steady_clock::time_point vpTreeTransformedEndQuery = steady_clock::now();
                double transformedQuery = duration_cast<std::chrono::nanoseconds>(vpTreeTransformedEndQuery - vpTreeTransformedStartQuery).count()/1000000.0;
                log2 << input_file.substr(input_file.find_last_of("/\\") + 1) << ','
                     << traces_file.substr(traces_file.find_last_of("/\\") + 1) << ','
                        << query.first << ','
                        << query.first.size() << ','
                        << strategyName << ','
                        << 0.0 << ','
                     << "VPTree+Embedding," << transformedQuery << ",,0" << std::endl;



                Matrix queryPoints(embedding_space.size(), 1);
                queryPoints.col(0) = x;
                steady_clock::time_point vpTreeTransformedStartQueryProposed = steady_clock::now();
                Matrixi indices;
                Matrix distances;
                kdtreeProposed.query(queryPoints, 10, indices, distances);
                steady_clock::time_point vpTreeTransformedEndQueryProposed = steady_clock::now();

                double transformedQueryProposed = duration_cast<std::chrono::nanoseconds>(vpTreeTransformedEndQueryProposed - vpTreeTransformedStartQueryProposed).count()/1000000.0;
                log2 << input_file.substr(input_file.find_last_of("/\\") + 1) << ','
                     << traces_file.substr(traces_file.find_last_of("/\\") + 1) << ','
                     << query.first << ','
                     << query.first.size() << ','
                     << strategyName << ','
                     << 0.0 << ','
                     << "VPKDTreeMinkowski+Embedding," << transformedQueryProposed << ",,0" << std::endl;
            }


            double noise = 0.0;
            std::cout << "    - trace: " << actualQuery << std::endl;
            Ranking<size_t> precomputedTraceRanking = performBenchmark(similarity, embedding_space, strategy, graphStrategy, map, actualQuery, noise, mapPath, log1);

            for (auto& noiser : query.second) {
                std::cout << "     - changed: " << noiser.first << std::endl;
                performBenchmark(similarity, embedding_space, strategy, graphStrategy, map, noiser.first, noiser.second, mapPath, log1, &precomputedTraceRanking);
            }
            log1 << std::flush;
        }
        std::cout << "Phase done!" << std::endl;


        delete graphStrategy;
        delete pathstrategy;
    }



#if MULTITHREADED_IMPL
    size_t id = 0;

    for (size_t i = 0; i<color_count; i++) {
        std::set<std::pair<std::string,std::string>> embedding_space;

        //globals per parameter
        PathEmbeddingStrategy strategy = color_entries[i].first;
        {

            benchmark_common_ground setting1;
            setting1.input_file = input_file;
            setting1.traces_file = traces_file;
            setting1.tuning_factor = tuning_factor;
            setting1.use_path_with_lambda_factor = use_path_lambda_factor;
            setting1.lambda = lambda;
            setting1.max_length = max_length;
            setting1.min_prob = min_prob;
            setting1.embeddingStrategy = strategy;
            toMultiThread.emplace_back(id++, strategy, epsilon, setting1, localMetric, finalGraph, admissibleCharList, seedError, this->noiseThreshold, this->finalLog, this->finalOriginalLog);
            pool.poolExecute([](MultithreadedBenchmarkForPooling* ptr) {return ptr->run();}, &toMultiThread[toMultiThread.size()-1]);

            benchmark_common_ground setting2 = setting1;
            setting2.tuning_factor = -1.0;             // disabling the tuning
            toMultiThread.emplace_back(id++, strategy, epsilon, setting2, localMetric, finalGraph, admissibleCharList, seedError, this->noiseThreshold, this->finalLog, this->finalOriginalLog);
            pool.poolExecute([](MultithreadedBenchmarkForPooling* ptr) {return ptr->run();}, &toMultiThread[toMultiThread.size()-1]);

        }

        /*
        std::cout << "Strategy " << (i+1) << " of " << color_count << ": " << color_entries[i].second.data() << std::endl;

        MultiplePathsEmbeddingStrategy* pathstrategy  = generatePathEmbeddingStrategyFromParameters(strategy);
        GraphEmbeddingStrategy *        graphStrategy = generateGraphEmbeddingStrategyFromParameters(strategy);


        std::cout << " * generating embeddings" << std::flush;
        ReadGraph::path_to_uembedding ptg;
        ReadGraph::path_to_uembedding ptu = (*pathstrategy)(finalGraph);
        ReadGraph::extractEmbeddingSpace(embedding_space, ptu);
        auto map = ReadGraph::generateStructuredEmbeddings(embedding_space, ptu);
        std::vector<struct path_info> mapPath;
        for (const auto& path : map) {
            mapPath.emplace_back(path.first);
        }
        std::cout << "... done" << std::endl;

        std::cout << " * generation precision metric" << std::flush;
        double precision_normalization = std::sqrt(set_of_path_similarity(mapPath, mapPath, similarity)*set_of_path_similarity(finalOriginalLog, finalOriginalLog, similarity));
        double precision = set_of_path_similarity(mapPath, finalOriginalLog, similarity, &log_quality, &precision_normalization)/precision_normalization;
        std::cout << "... done" << std::endl;
        std::cout << "   precision = " << precision << std::endl;

        std::cout << " * starting with query analysis!" << std::endl;
        for (const auto& query : finalLog) {
            std::string actualQuery = query.path;
            double noise = 0.0;
            std::cout << "    - trace: " << actualQuery << std::endl;

            performBenchmark(similarity, probSimMetric, embedding_space, strategy,
                                               graphStrategy, map, actualQuery, noise, mapPath);

            for (auto& noiser : traceNoiser) {
                std::string alteredQuery;
                do {
                    if (alteredQuery == "bcc")
                        std::cerr << "DEBUG" << std::endl;
                    alteredQuery = noiser.alter(query.path);
                } while ((alteredQuery == query.path) || (alteredQuery.empty()));
                std::cout << "      : alteredAs: " << alteredQuery << std::endl;

                performBenchmark(similarity, probSimMetric, embedding_space, strategy, graphStrategy, map, actualQuery, noiser.noiseThreshold, mapPath);
            }
        }
        std::cout << "Phase done!" << std::endl;

        */
        //delete graphStrategy;
        //delete pathstrategy;
    }

    std::string logFile1 = (results_folder / "log_proposed_main.csv").string();
    std::ofstream log1(logFile1, std::ofstream::out);
    std::string logFile2 = (results_folder / "log_proposed_additional.csv").string();
    std::ofstream log2(logFile2, std::ofstream::out);

    LogBenchmark::log_record_element_per_query(log1);
    LogBenchmark::log_record_element_per_log(log2);
    for (auto& key : pool.foreach()) {
        log1 << std::flush;
        log2 << std::flush;
        LogBenchmark::log_record_element_per_query(log1, key.first);
        LogBenchmark::log_record_element_per_log(log2, key.second);
        log1 << std::flush;
        log2 << std::flush;
    }
#endif
}

void ConfigurationFile::convertLog(const std::vector<Transaction<TimestampedEvent>> &currentLog,
                                   std::vector<struct path_info> &final)  {
    for (const auto& trace : currentLog) {
        std::string stringBuilder;
        for (const auto& str : trace) {
            stringBuilder += action_to_single_char.getValue(str.event_name);
        }
        final.emplace_back(1.0, stringBuilder, std::vector<size_t>{});
    }
}

#if 0

std::pair<size_t, double> getSimilarity(Ranking<size_t>& expectedRanking, Ranking<size_t>& pathRanking) {
    return std::make_pair(1, expectedRanking.normalizedRank(pathRanking, [](size_t x, size_t y){return x==y;}));
}
std::pair<size_t, double> getSpearman(Ranking<size_t>& expectedRanking, Ranking<size_t>& pathRanking) {
    return std::make_pair(1, expectedRanking.SpearmanCorrelationIndex(pathRanking, 1.0));
}
std::pair<size_t, double> getProposed(Ranking<size_t>& expectedRanking, Ranking<size_t>& pathRanking) {
    return std::make_pair(2, minimum_edit_maximum_substring(pathRanking, expectedRanking));
}
#endif


Ranking<size_t>
ConfigurationFile::performBenchmark(const LevensteinSimilarity &similarity/*, DistanceExpressionEvaluator *probSimDistance*/,
                                    std::set<std::pair<std::string, std::string>> &embedding_space,
                                    PathEmbeddingStrategy &strategy, GraphEmbeddingStrategy *graphStrategy,
                                    const std::unordered_map<struct path_info, Eigen::VectorXd> &map,
                                    std::string &query, double noise,
                                    std::vector<struct path_info>& pathsOrder, std::ostream& log_quality, Ranking<size_t>* precomputedTraceRanking)  {
    assert((noise == 0.0) || precomputedTraceRanking );
    std::string strategyName{magic_enum::enum_name(strategy).data()};
    unsigned int threadExperiment = (noise != 0.0) ? 3 : 2;
    size_t  querySize = query.size();
    ReadGraph g = ReadGraph::fromString(query, 1.0);
    auto tmp = (*graphStrategy)(g);
    auto x = ReadGraph::generateStructuredEmbedding(embedding_space, tmp);
    Ranking<size_t> pathRanking, expectedRanking;
    ///std::map<double, std::vector<size_t>> rankingMap;
    size_t j = 0;
    double forGeneralization = -1.0;
    size_t genIdx = 0;
    for (const auto& paths : map) {
        double sc = x.dot(paths.second);
        pathRanking.addScore(j, sc);
        double similarity = 1.0/(GeneralizedLevensteinDistance(paths.first.path, query) / 5.0 + 1.0);
        if (similarity > forGeneralization) {
            genIdx = j;
            forGeneralization = similarity;
        }

        double finalScore = (paths.first.probability* similarity);
        expectedRanking.addScore(j, finalScore);
        ///rankingMap[finalScore].emplace_back(j);

        j++;
    }

    log_ranking(query, noise, log_quality, precomputedTraceRanking, strategyName, querySize, pathRanking, expectedRanking, forGeneralization, false);
    {
        auto localE = doDulcior(expectedRanking, pathsOrder.size());
        auto localP = doDulcior(pathRanking, pathsOrder.size());
        Ranking<size_t> localTR;
        if (precomputedTraceRanking) {
            localTR = doDulcior(*precomputedTraceRanking, pathsOrder.size());
        }
        log_ranking(query, noise, log_quality, precomputedTraceRanking ? &localTR : nullptr, strategyName, querySize, localP, localE, forGeneralization, true);
    }


#if 0
    //log_quality.emplace_back(input_file, traces_file, query, query.size(), strategy, noise, false, tuning_factor, use_path_lambda_factor, lambda, max_length, min_prob, LogDependantQualityMetricTypes::LogGeneralizationMetric, forGeneralization, "", 0);

    ///for (const auto& x : rankingMap) assert(x.second.size() == 1); // Imposing that there are no elements with the same scoring value
    const auto ls = [&similarity, &pathsOrder](const size_t& left, const size_t& right) { return 1.0/(GeneralizedLevensteinDistance(pathsOrder.at(left).path, pathsOrder.at(right).path) / 5.0 + 1.0) /* similarity.similarity(pathsOrder.at(left).path, pathsOrder.at(right).path)*/; };
    bool isMultiThreaded = true;

    log_stats(map, query, noise, log_quality, precomputedTraceRanking, strategyName, querySize, isMultiThreaded,
              false, threadExperiment, pathRanking,
              expectedRanking);

    {
        Ranking<size_t> localExpected = doDulcior(expectedRanking, pathsOrder.size());
        Ranking<size_t> localPath = doDulcior(pathRanking, pathsOrder.size());

        log_stats(map, query, noise, log_quality, precomputedTraceRanking, strategyName, querySize, isMultiThreaded,
                  true, threadExperiment, localPath,localExpected);
        /*double rankingDistance = expectedRanking.normalizedRank(pathRanking, ls);
        double spearman = expectedRanking.SpearmanCorrelationIndex(pathRanking, 1.0);
        double proposedMetric = minimum_edit_maximum_substring(pathRanking, expectedRanking);*/

        /*MultithreadWrap<std::pair<size_t, double>> pool{threadExperiment, true};

        // TODO: noise
        // double rankingDistance = expectedRanking.normalizedRank(pathRanking, ls);
        pool.poolExecute(getSpearman, expectedRanking, pathRanking);
        pool.poolExecute(getProposed, expectedRanking, pathRanking);
        double spearman;
        double proposedMetric;
        double rankingDistance;
        for (const auto& cp : pool.foreach()) {
            if (cp.first == 1)
                spearman = cp.second;
            else if (cp.first == 2)
                proposedMetric = cp.second;
            else if (cp.first == 0) {
                assert(noise == 0.0);
                rankingDistance = cp.second;
            }
        }

        output_quality.emplace_back(input_file, traces_file, query, query.size(), strategy, noise, rankingDistance, spearman, proposedMetric, true, tuning_factor, use_path_lambda_factor, lambda, max_length, min_prob);
        size_t i = 0;
        i++;*/
    }
#endif

    return pathRanking;
}

void ConfigurationFile::log_ranking(const std::string &query, double noise, std::ostream &log_quality,
                                                Ranking<size_t> *precomputedTraceRanking,
                                                const std::string &strategyName, size_t querySize,
                                                Ranking<size_t> &pathRanking, Ranking<size_t> &expectedRanking,
                                                double forGeneralization, double dulcior) const {
    log_quality << input_file.substr(input_file.find_last_of("/\\") + 1) << ','
                << traces_file.substr(traces_file.find_last_of("/\\") + 1) << ','
                << query << ','
                << querySize << ','
                << strategyName << ','
                << noise << ','
            << (dulcior ? "T," : "F,")
                << tuning_factor << ','
                << (use_path_lambda_factor ? "T," : "F,")
                << lambda << ','
                << max_length << ','
                << min_prob
                << ",LogGeneralizationMetric," << forGeneralization << ",,0" << std::endl;

    log_quality << input_file.substr(input_file.find_last_of("/\\") + 1) << ','
                << traces_file.substr(traces_file.find_last_of("/\\") + 1) << ','
                << query << ','
                << querySize << ','
                << strategyName << ','
                << noise << ','
                         << (dulcior ? "T," : "F,")
                << tuning_factor << ','
                << (use_path_lambda_factor ? "T," : "F,")
                << lambda << ','
                << max_length << ','
                << min_prob
                << ",Spearman," << expectedRanking.SpearmanCorrelationIndex(pathRanking, 1.0) << ",,0" << std::endl;

    log_quality << input_file.substr(input_file.find_last_of("/\\") + 1) << ','
                << traces_file.substr(traces_file.find_last_of("/\\") + 1) << ','
                << query << ','
                << querySize << ','
                << strategyName << ','
                << noise << ','
                         << (dulcior ? "T," : "F,")
                << tuning_factor << ','
                << (use_path_lambda_factor ? "T," : "F,")
                << lambda << ','
                << max_length << ','
                << min_prob
                << ",ProposedMetric," << minimum_edit_maximum_substring(pathRanking, expectedRanking) << ",,0" << std::endl;

    if (noise != 0.0) {
        double noise1 = precomputedTraceRanking->SpearmanCorrelationIndex(pathRanking, 1.0);
        double noise2 = minimum_edit_maximum_substring(pathRanking, *precomputedTraceRanking);
        if (std::isinf(noise1)) {
            std::cerr << "ERROR!" << std::endl;
            double noise1 = precomputedTraceRanking->SpearmanCorrelationIndex(pathRanking, 1.0);
            double noise2 = minimum_edit_maximum_substring(pathRanking, *precomputedTraceRanking);
        }
        ///assert(precomputedTraceRanking);
        log_quality << input_file.substr(input_file.find_last_of("/\\") + 1) << ','
                    << traces_file.substr(traces_file.find_last_of("/\\") + 1) << ','
                    << query << ','
                    << querySize << ','
                    << strategyName << ','
                    << noise << ','
                             << (dulcior ? "T," : "F,")
                    << tuning_factor << ','
                    << (use_path_lambda_factor ? "T," : "F,")
                    << lambda << ','
                    << max_length << ','
                    << min_prob
                    << ",WithNoiseDistanceSpearman," <<  precomputedTraceRanking->SpearmanCorrelationIndex(pathRanking, 1.0) << ",,0" << std::endl;

        log_quality << input_file.substr(input_file.find_last_of("/\\") + 1) << ','
                    << traces_file.substr(traces_file.find_last_of("/\\") + 1) << ','
                    << query << ','
                    << querySize << ','
                    << strategyName << ','
                    << noise << ','
                             << (dulcior ? "T," : "F,")
                    << tuning_factor << ','
                    << (use_path_lambda_factor ? "T," : "F,")
                    << lambda << ','
                    << max_length << ','
                    << min_prob
                    << ",WithNoiseDistanceProposedMetric," <<  minimum_edit_maximum_substring(pathRanking, *precomputedTraceRanking) << ",,0" << std::endl;
    }
}

#if 0
void
ConfigurationFile::log_stats(const std::unordered_map<struct path_info, Eigen::VectorXd> &map, const std::string &query,
                             double noise, std::ostream &log_quality, Ranking<size_t> *precomputedTraceRanking,
                             const std::string &strategyName, size_t querySize, bool isMultiThreaded, bool isDulcior,
                             unsigned int &threadExperiment, Ranking<size_t> &pathRanking,
                             Ranking<size_t> &expectedRanking) const {
    MultithreadWrap<std::pair<size_t, double>> pool{threadExperiment, isMultiThreaded};
    if (noise != 0.0) {
        assert(precomputedTraceRanking);
        pool.poolExecute(getSimilarity, precomputedTraceRanking->topK(((double)map.size())/3.0*2.0), pathRanking.topK((((double)map.size())/3.0*2.0)));
    }
    pool.poolExecute(getSpearman, expectedRanking, pathRanking);
    pool.poolExecute(getProposed, expectedRanking, pathRanking);
    for (const auto& cp : pool.foreach()) {
        if (cp.first == 1) {
            log_quality << input_file << ','
                        << traces_file << ','
                        << query << ','
                        << querySize << ','
                        << strategyName << ','
                        << noise << ','
                        << (isDulcior ? "T," : "F,")
                        << tuning_factor << ','
                        << (use_path_lambda_factor ? "T," : "F,")
                        << lambda << ','
                        << max_length << ','
                        << min_prob
                           << ",Spearman," << cp.second << ",,0" << std::endl;
        }
        else if (cp.first == 2) {
            log_quality << input_file << ','
                        << traces_file << ','
                        << query << ','
                        << querySize << ','
                        << strategyName << ','
                        << noise << ','
                        << (isDulcior ? "T," : "F,")
                        << tuning_factor << ','
                        << (use_path_lambda_factor ? "T," : "F,")
                        << lambda << ','
                        << max_length << ','
                        << min_prob
                        << ",ProposedMetric," << cp.second << ",0" << std::endl;
        }
        else if (cp.first == 0) {
            log_quality << input_file << ','
                        << traces_file << ','
                        << query << ','
                        << querySize << ','
                        << strategyName << ','
                        << noise << ','
                        << (isDulcior ? "T," : "F,")
                        << tuning_factor << ','
                        << (use_path_lambda_factor ? "T," : "F,")
                        << lambda << ','
                        << max_length << ','
                        << min_prob
                           << ",TracesSetWeightedSimilarityWithNoise," << cp.second << ",,0" << std::endl;
        }
    }
}
#endif

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
                        auto ptr = new DistanceExpressionEvaluator(kv.second.as<std::string>());
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



