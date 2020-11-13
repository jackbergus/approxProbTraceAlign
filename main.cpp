
////#define DULCIOR

#include <embeddings/path_embedding/MultiplePathsEmbeddingStrategy.h>
#include <embeddings/graph_embedding/GraphEmbeddingStrategy.h>
#include <z3.h>
#include "ReadGraph.h"
#include "RegexVisitor.h"

double score(const Eigen::VectorXd& left, const Eigen::VectorXd& right) {
    return left.dot(right);
}
double scoreNormalized(const Eigen::VectorXd& left, const Eigen::VectorXd& right) {
    return left.dot(right) / std::sqrt(left.dot(left) * right.dot(right));
}

Eigen::IOFormat CleanFmt(4, 0, "", ",", "", "", "[", "]");



void doTestDemultiplexed(double stringWeight, MultiplePathsEmbeddingStrategy* strategy, GraphEmbeddingStrategy* testStrategy) {
    std::set<std::pair<std::string,std::string>> embedding_space;
    ReadGraph t{"matrix.txt"};
    ReadGraph::path_to_uembedding ptg;
    ReadGraph::path_to_uembedding ptu = (*strategy)(t);
    ReadGraph::extractEmbeddingSpace(embedding_space, ptu);
    auto map = ReadGraph::generateStructuredEmbeddings(embedding_space, ptu);
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    ReadGraph g = ReadGraph::fromString("abc", stringWeight);
    auto tmp = (*testStrategy)(g);
    auto x = ReadGraph::generateStructuredEmbedding(embedding_space, tmp);


    for (const auto& y : map) {
        std::cout << "\tabc vs. " << y.first << " = " << score(x, y.second) << ", normalized = " << scoreNormalized(x, y.second) << std::endl;
    }
}

#include <sstream>
#include <benchmarking/Ranking.h>

Ranking<std::string> doTestDemultiplexed2(double stringWeight, MultiplePathsEmbeddingStrategy* strategy, GraphEmbeddingStrategy* testStrategy, const std::string& matrix="matrix6.txt", const std::string& caba="caba") {
    std::set<std::pair<std::string,std::string>> embedding_space;
    ReadGraph t{matrix};
    ReadGraph::path_to_uembedding ptg;
    ReadGraph::path_to_uembedding ptu = (*strategy)(t);
    ReadGraph::extractEmbeddingSpace(embedding_space, ptu);
    auto map = ReadGraph::generateStructuredEmbeddings(embedding_space, ptu);
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    ReadGraph g = ReadGraph::fromString(caba, stringWeight);
    auto tmp = (*testStrategy)(g);
    auto x = ReadGraph::generateStructuredEmbedding(embedding_space, tmp);
    std::cout << "q=" << x.format(CleanFmt) << std::endl << std::endl;
    std::map<double, std::string> results;
    Ranking<std::string> pathRanking;
    for (const auto& y : map) {
        double sc = score(x, y.second);
        pathRanking.addScore(y.first, sc);
        std::ostringstream iss;
        iss << y.second.format(CleanFmt);
        results[sc] += ("\n\t" + caba + " vs. " + y.first + " = " + std::to_string(sc) +" \n" + iss.str() +"\n\n");
    }
    for (auto it = results.rbegin(); it != results.rend(); it++) {
        std::cout << "SCORE = " << it->first << std::endl << '\t' << it->second << std::endl;

    }
#ifndef DULCIOR
    return pathRanking;
#else
    Ranking<std::string> finalRanking;
    for (const auto& y: map) {
        finalRanking.addScore(y.first, pathRanking.getRanking(y.first));
    }
    return finalRanking;
#endif
}



void doMultiplePetriDemultiplexedTest(double stringWeight, GraphEmbeddingStrategy*         ges) {
    std::set<std::pair<std::string,std::string>> embedding_space;
    ReadGraph g1{"matrix.txt"};
    ReadGraph g2{"matrix2.txt"};
    ReadGraph g3{"matrix3.txt"};
    ReadGraph g = ReadGraph::fromString("abc", stringWeight);

    const ReadGraph::unstructured_embedding &uv1 = (*ges)(g1);
    const ReadGraph::unstructured_embedding &uv2 = (*ges)(g2);
    const ReadGraph::unstructured_embedding &uv3 = (*ges)(g3);
    const ReadGraph::unstructured_embedding &uvg = (*ges)(g);
    ReadGraph::extractEmbeddingSpace(embedding_space, uv1);
    ReadGraph::extractEmbeddingSpace(embedding_space, uv2);
    ReadGraph::extractEmbeddingSpace(embedding_space, uv3);

    auto v1 = ReadGraph::generateStructuredEmbedding(embedding_space, uv1);
    auto v2 = ReadGraph::generateStructuredEmbedding(embedding_space, uv2);
    auto v3 = ReadGraph::generateStructuredEmbedding(embedding_space, uv3);
    auto t = ReadGraph::generateStructuredEmbedding(embedding_space, uvg);

    std::cout << "abc vs. matrix1  = " << score(v1, t) << ", normalized = " << scoreNormalized(v1, t) << std::endl;
    std::cout << "abc vs. matrix2  = " << score(v2, t) << ", normalized = " << scoreNormalized(v2, t) << std::endl;
    std::cout << "abc vs. matrix3  = " << score(v3, t) << ", normalized = " << scoreNormalized(v3, t) << std::endl;


    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
}

#include <embeddings/path_embedding//EmbedPahtsOverSingleGraphStrategy.h>
#include <embeddings/labelled_paths/NodesWithTransitiveEdgeCost.h>

void testsDemultiplexed() {
#if 0
    {
        MultiplePathsEmbeddingStrategy* mpes = new EmbedPathsOverSingleGraphStrategy<OnlyTransitiveEdgesCost>(1.0, true, 100, 0.00001);
        GraphEmbeddingStrategy*         ges  = new TransitiveClosureGraphStrategy<OnlyTransitiveEdgesCost>(1.0, 100);
        doTestDemultiplexed(1.0, mpes, ges);
        doTestDemultiplexed(0.3, mpes, ges);
        delete mpes;
        delete ges;
    }

    {
        MultiplePathsEmbeddingStrategy* mpes = new EmbedPathsOverSingleGraphStrategy<OnlyTransitiveEdgesCost>(0.7, true, 100, 0.00001);
        GraphEmbeddingStrategy*         ges  = new TransitiveClosureGraphStrategy<OnlyTransitiveEdgesCost>(0.7, 100);
        doTestDemultiplexed(1.0, mpes, ges);
        doTestDemultiplexed(0.3, mpes, ges);
        delete mpes;
        delete ges;
    }


    {
        MultiplePathsEmbeddingStrategy* mpes = new EmbedPathsOverSingleGraphStrategy<NodesWithTransitiveEdgeCost>(1.0, true, 100, 0.00001);
        GraphEmbeddingStrategy*         ges  = new TransitiveClosureGraphStrategy<NodesWithTransitiveEdgeCost>(1.0, 100);
        doTestDemultiplexed(1.0, mpes, ges);
        doTestDemultiplexed(0.3, mpes, ges);
        delete mpes;
        delete ges;
    }

    {
        MultiplePathsEmbeddingStrategy* mpes = new EmbedPathsOverSingleGraphStrategy<NodesWithTransitiveEdgeCost>(0.7, true, 100, 0.00001);
        GraphEmbeddingStrategy*         ges  = new TransitiveClosureGraphStrategy<NodesWithTransitiveEdgeCost>(0.7, 100);
        doTestDemultiplexed(1.0, mpes, ges);
        doTestDemultiplexed(0.3, mpes, ges);
        delete mpes;
        delete ges;
    }


//#if 1
    {
        GraphEmbeddingStrategy*         ges  = new TransitiveClosureGraphStrategy<OnlyTransitiveEdgesCost>(1.0, 100);
        doMultiplePetriDemultiplexedTest(1.0,  ges);
        doMultiplePetriDemultiplexedTest(0.3, ges);
        delete ges;
    }
    {
        GraphEmbeddingStrategy*         ges  = new TransitiveClosureGraphStrategy<OnlyTransitiveEdgesCost>(0.7, 100);
        doMultiplePetriDemultiplexedTest(1.0,  ges);
        doMultiplePetriDemultiplexedTest(0.3, ges);
        delete ges;
    }

    {
        GraphEmbeddingStrategy*         ges  = new TransitiveClosureGraphStrategy<NodesWithTransitiveEdgeCost>(1.0, 100);
        doMultiplePetriDemultiplexedTest(1.0,  ges);
        doMultiplePetriDemultiplexedTest(0.3, ges);
        delete ges;
    }
    {
        GraphEmbeddingStrategy*         ges  = new TransitiveClosureGraphStrategy<NodesWithTransitiveEdgeCost>(0.7, 100);
        doMultiplePetriDemultiplexedTest(1.0,  ges);
        doMultiplePetriDemultiplexedTest(0.3, ges);
        delete ges;
    }
//#endif
#endif

}



#include <fstream>
#include <regexLexer.h>
#include <regexParser.h>
#include <benchmarking/Ranking.h>
#include "Regex.h"
#include "ThomsonNFA.h"
#include "utils/AlterString.h"
#include "distances/path/GenericStringCostFunction.h"
#include "RankingTest.h"


void parse() {
    std::ifstream file("test2.txt", std::ifstream::binary);
    // Convert the file into ANTLR's format.
    antlr4::ANTLRInputStream stream = antlr4::ANTLRInputStream(file);

    // Give the input to the lexer.
    regexLexer lexer{&stream};
    // Generate the tokens.
    antlr4::CommonTokenStream tokens(&lexer);

    file.close();

    tokens.fill();

    // Create the translation that will parse the input.
    regexParser parser(&tokens);
    parser.setBuildParseTree(true);
    RegexVisitor visitor;


    ThomsonNFA nfa{visitor.visit(parser.regex()).as<Regex<std::string>*>()};
    nfa.generateGraph(1.0).printGraph();

}

void doAlter() {

    std::string seed{"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    AlterString as{seed, 0.3};
    for (size_t i = 0; i<10; i++) {
        std::cout << as.alter("abcdefghijklmn") << std::endl;
    }
}

void generatePaths() {
    ReadGraph t{"data/matrix4.txt"};
    for (const auto& x : t.iterateOverPaths(false, 20, 0.000001)) {
        std::cout << x << std::endl;
    }
}

#include <ostream>
#include <distances/strings/LevensteinDistance.h>
#include <benchmarking/BenchmarkStrategy.h>
#include <benchmarking/BenchmarkConfiguration.h>
#include <distances/strings/LevensteinSimilarity.h>

struct BenchmarkGenericStringCostFunction : public BenchmarkStrategy {

    GenericStringCostFunction<LevensteinDistance> pathSimilarity{1, 1, 1};

    BenchmarkGenericStringCostFunction(
                                       bool doNotVisitLoopsTwice, size_t maxPathLength, const double minimumPathCost)
            : BenchmarkStrategy("--error: must provide a dataset name--", "--error: must provide a configuration--", doNotVisitLoopsTwice, maxPathLength, minimumPathCost,
                                [](const ReadGraphRankingStruct& left, const ReadGraphRankingStruct& right) {
                                    return GeneralizedLevensteinDistance(left.readGraphPath, right.readGraphPath);
            }){}

    void scorePath(const std::string &query, ReadGraph *, const struct path_info &path_from_graph,
                   PathRanking &pathRanking, double &benchmark_time) override {
        auto start = std::chrono::system_clock::now();
        double score = pathSimilarity.distance(query, path_from_graph);
        auto end = std::chrono::system_clock::now();
        pathRanking.addScore(path_from_graph, score);
        benchmark_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    }

    void scoreGraph(const std::string &query, ReadGraph *rg, const std::string &graph_name,
                    Ranking<ReadGraphRankingStruct> &graphRanking, double &benchmark_time) override {
        double cost = -1000000000000000000000.0;
        struct path_info x;
        auto start = std::chrono::system_clock::now();
        for (const struct path_info& path : rg->iterateOverPaths(this->doNotVisitLoopsTwice, this->maxPathLength, this->minimumPathCost)) {
            double currentCost = pathSimilarity.distance(query, path);
            if (currentCost > cost) {
                cost = currentCost;
                x = path;
            }
        }
        auto end = std::chrono::system_clock::now();
        graphRanking.addScore({x.path, rg}, cost);
        benchmark_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    }

};

class IdiotRanking : public RankingTest {
    Ranking<size_t> intRank;
    std::function<double(double)> f = [](double x){ return 0.0; };
public:
    IdiotRanking(bool doNotVisiPahtsTwive, size_t maxPathLength, double minimumPathCost) : RankingTest(
            doNotVisiPahtsTwive, maxPathLength, minimumPathCost) {}

    void loadPathEmbedding(const struct path_info &path, size_t pathId, ReadGraph &sourceGraph,
                           const std::string &query) override {
        intRank.addScore(pathId, path.cost/(GeneralizedLevensteinDistance(path.path, query)/5.0+1.0));
    }

    void indexingEmbedding() override {}

    Ranking<size_t> generateRank(const std::string& query, size_t topK) override {
        return intRank.topK(topK, f);
    }
};

class ProposedRanking : public RankingTest {
    const double lambda;
    EmbedPathsOverSingleGraphStrategy<NodesWithTransitiveEdgeCost> eposgs;
    TransitiveClosureGraphStrategy<NodesWithTransitiveEdgeCost>    gEmbed;
    std::vector<std::pair<struct path_info, ReadGraph*>> storePathsWithGraphs;
    std::set<std::pair<std::string, std::string>> embeddingSpace;
    std::vector<Eigen::VectorXd> finalEmbedding;
    std::unordered_map<ReadGraph*, ReadGraph::path_to_uembedding> readGraphMap;

public:
    ProposedRanking(double tuning_factor, bool doNotVisiPahtsTwive, size_t maxPathLength, double minimumPathCost, const double lambda)
            : RankingTest(doNotVisiPahtsTwive, maxPathLength, minimumPathCost), lambda(lambda), eposgs(tuning_factor, lambda, doNotVisiPahtsTwive, maxPathLength, minimumPathCost), gEmbed(tuning_factor, lambda, maxPathLength) {}

    void loadPathEmbedding(const struct path_info &path, size_t pathId, ReadGraph &sourceGraph,
                           const std::string &query) override {
        assert(pathId == storePathsWithGraphs.size());
        /*const auto& x = */storePathsWithGraphs.emplace_back(path, &sourceGraph);
        auto it = readGraphMap.find(&sourceGraph);
        ///std::cout << pathId << ": " << path.cost << std::endl;
        if (it == readGraphMap.end()) {
            ReadGraph::path_to_uembedding multiEmbeddings = eposgs(sourceGraph);
            ReadGraph::extractEmbeddingSpace(embeddingSpace, multiEmbeddings);
            it = readGraphMap.emplace(&sourceGraph, multiEmbeddings).first;
        }
    }

    void indexingEmbedding() override {
        std::unordered_map<ReadGraph*, std::unordered_map<std::string, Eigen::VectorXd>>  localFinalEmbedding;
        for (const auto& it : readGraphMap) {
            for (const auto& inner : ReadGraph::generateStructuredEmbeddings(embeddingSpace, it.second)) {
                assert(localFinalEmbedding[it.first].insert(inner).second);
            }
        }
        for (auto & storePathsWithGraph : storePathsWithGraphs){
            finalEmbedding.emplace_back(localFinalEmbedding.at(storePathsWithGraph.second).at(storePathsWithGraph.first.path));
        }
    }

    Ranking<size_t> generateRank(const std::string& query, size_t topK) override {
        ReadGraph queryGraph = ReadGraph::fromString(query, 1.0);
        ReadGraph::unstructured_embedding tmp = gEmbed(queryGraph);
        Eigen::VectorXd x = ReadGraph::generateStructuredEmbedding(embeddingSpace, tmp);
        std::cout << x.format(CleanFmt) << std::endl;
        Ranking<size_t> toret;
        for (size_t i = 0, n = finalEmbedding.size(); i<n; i++) {
            //std::cout << i << ": " << std::sqrt(score(finalEmbedding[i], finalEmbedding[i])) << std::endl;
            /// TODO: std::cout << i << ": " << finalEmbedding[i].format(CleanFmt) << std::endl;
            toret.addScore(i, score(x,x) + score(finalEmbedding[i], finalEmbedding[i]) - 2*score(x, finalEmbedding[i]));
        }
        return toret;
    }
};


void simpleBenchmark() {

    BenchmarkGenericStringCostFunction conf{false, 100, 0.000001};
    BenchmarkConfiguration configuration{&conf};
    configuration.loadGraph({"test2.txt", true, 1.0, input_format::REGEX});
    std::cout << configuration.getGraphCollectionName() << std::endl;
    //configuration.benchmarkMultiplePathsFromSameGraph("test.txt");
    configuration.benchmarkMultiplePathsFromSameGraphWithNoise("test2.txt", 0.3);
    std::cout << configuration << std::endl;
}

Ranking<std::string> generate_example_expected_ranking(const std::string& matrix = "matrix6.txt", const std::string& caba = "caba") {
    ReadGraph g{matrix};
    std::vector<std::pair<double,std::string>> paths;
    for (const auto& path:
            g.iterateOverPaths(false, 5,std::numeric_limits<double>::epsilon()*2)) {
        std::cout << "path = " << path.path << " prob = " << path.cost << std::endl;
        paths.emplace_back( path.cost, path.path);
    }
    /*TransitiveClosureGraphStrategy<NodesWithTransitiveEdgeCost> embedding(-1, 0.7,  5);
     ReadGraph::unstructured_embedding l = embedding(g);*/

    /*ReadGraph g2 = ReadGraph::fromString("caba", 1.0);
    ReadGraph::unstructured_embedding l2 = embedding(g2);*/

    std::map<double, std::vector<std::string>> rankingMap;
    Ranking<std::string> expectedRanking;
    for (const auto& x : paths) {
        double distance = GeneralizedLevensteinDistance(x.second, caba);
        double score = 1.0/(distance/5.0+1.0);
        expectedRanking.addScore(x.second, x.first* score);
        std::cout << x.second << ": probability=" << x.first << " distance: " << distance << " similarity: " << score << " RANKING: " << x.first * score << std::endl;
        rankingMap[x.first * score ].emplace_back(x.second);
    }
    size_t maxRank = 1;
    for (auto it = rankingMap.rbegin(); it != rankingMap.rend(); it++) {
        std::cout << maxRank << " [" << it->first << "]" << std::endl;
        for (const auto & y : it->second) {
            std::cout << '\t' << y << " ";
        }
        std::cout << std::endl;
        maxRank++;
    }
#ifndef DULCIOR
    return expectedRanking;
#else
    Ranking<std::string> finalRanking;
    for (const auto& y: paths) {
        finalRanking.addScore(y.second, expectedRanking.getRanking(y.second));
    }
    return finalRanking;
#endif
    ///exit(1);
}

struct algorithm_map {
    size_t index;
    std::vector<size_t> edits;
    std::vector<size_t> sequence;
    double precision = 0.0;
    double simialrity = 0.0;
    double score = 0.0;

    algorithm_map(const algorithm_map&) = default;
    algorithm_map& operator=(const algorithm_map& ) = default;
    algorithm_map(size_t index, const std::vector<size_t> &edits, const std::vector<size_t> &sequence) : index(index),
                                                                                                         edits(edits),
                                                                                                         sequence(
                                                                                                                 sequence) {};
    algorithm_map() {}
};

void
doBenchmark(const std::string &matrix, const std::string &query, double tuning_factor, double lambda, double min_path,
            size_t max_path);


void rectify(struct algorithm_map& x) {}
void rectify2(struct algorithm_map& x) {
    size_t N = x.edits.size();
    if ((N>=2) && ((x.edits[N-1] == x.edits[N-2]))) {  // Removing the clashes of slight changes in the order. Nevertheless, I will analyse the monotonicity, so this information won't be discarded
        x.edits.pop_back();
    }
}

double minimum_edit_maximum_substring(const std::vector<size_t>&  ranking) {

    std::cout << "{{{INPUT: ";
    for (const size_t& x : ranking)
        std::cout << x << ", ";
    std::cout << "}}}a" << std::endl;

    std::vector<algorithm_map> backup;

    std::vector<size_t> expected(ranking.size());
    std::iota(expected.begin(), expected.end(), 1);
    double S = ranking.size();

    {
        std::unordered_map<size_t, algorithm_map> C;
        C[0] = {};
        C[0].index = 0;
        for (size_t i = 0, N = ranking.size(); i<N; i++) {
            size_t current  = ranking[i];
            if ((i == 0) || (ranking[i] > ranking[i-1])) {
                for (auto& kv : C) kv.second.sequence.emplace_back(ranking[i]);
                for (size_t j = ranking[i-1]+1, M = ranking[i]-1; j<M; j++) {
                    for (auto& kv : C) kv.second.edits.emplace_back(j);
                }
                for (auto& kv : C) rectify(kv.second);
            } else {
                for (const auto& kv : C) backup.emplace_back(kv.second);
                for (auto& kv: C) {
                    kv.second.edits.emplace_back(ranking[i]);
                    kv.second.sequence.emplace_back(ranking[i]);
                    rectify2(kv.second);
                }
                C[i] = {};
                C[i].index = i;
                C[i].sequence.emplace_back(ranking[i]);
            }
        }
        for (const auto& kv : C) backup.emplace_back(kv.second);
    }

    double max = -1; long long int index = -1;
    for (size_t i = 0, N = backup.size(); i<N; i++) {
        algorithm_map& ref = backup[i];

        // /////////////////////////////////////////////////////////
        // Discard the sequence if it is prevalently decrementing //
        // /////////////////////////////////////////////////////////
        long long int summation = 0;
        for (size_t j = 1, M = ref.sequence.size(); j<M; j++) {
            long long int difference = (((long long int)ref.sequence[j])-((long long int)ref.sequence[j-1]));
            if ((difference < 0) && (summation > 0)) summation = 0;
            summation += (((long long int)ref.sequence[j])-((long long int)ref.sequence[j-1]));
            if ((summation < 0) && (difference>0)) break;
        }
        if (summation <= 0) continue;
        // /////////////////////////////////////////////////////////
        // /////////////////////////////////////////////////////////
        // /////////////////////////////////////////////////////////

        ref.precision = ((double)ref.sequence.size())/S;
        ref.simialrity = 1.0/(((double)ref.edits.size())/5.0+1.0);
        ref.score = ref.precision * ref.simialrity;
        if ((ref.score > max ) || ((ref.score == max) && (ref.simialrity > backup[index].simialrity)) ||((ref.score == max) && (ref.simialrity == backup[index].simialrity) && (ref.precision > backup[index].precision)) ) {
            max = ref.score;
            index = i;
        }
    }


    if ((max < 0) || (index == -1)) {
        std::cout<< "There is no good sequence" << std::endl;
        return 0;
    }
    std::cout << "Score " << backup[index].score << " for: ";
    for (const auto& sol : backup[index].sequence)
        std::cout << sol << ", ";
    std::cout << "from index " << index <<  std::endl;
    return backup[index].score;

}

void benchmarking() {

    /*minimum_edit_maximum_substring({3,7,1,2,4,6,5,8});
    minimum_edit_maximum_substring({1,4,5,7,8,2,3,6});
    minimum_edit_maximum_substring({8,7,6,5,4,2,3,1});
    exit(2);*/

    ///generate_example_expected_ranking();
#define DEBUG
#ifdef DEBUG
    if (0){
        /*TransitiveClosureGraphStrategy<NodesWithTransitiveEdgeCost> tcgs(0.0001, 0.7, 5);
        std::set<std::pair<std::string,std::string>> embedding_space;
        ReadGraph g1{"matrix5.txt"};
        g1.printGraph();*/


        /*ProposedRanking ir{-1, false, 5, std::numeric_limits<double>::epsilon()*2, 0.7};
        ir.loadGraph({"matrix5.txt", true, 1.0, input_format::READGRAPH});
        std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << ir.generateRanking("caba", 10) << std::endl;
        exit(1);
        */
        /*const ReadGraph::unstructured_embedding &uv1 = tcgs(g1);
        ReadGraph::extractEmbeddingSpace(embedding_space, uv1);
        exit(1);*/

        std::vector<size_t> target{1,2,3,4,5,6,7,8};
        std::vector<size_t> returned1{1,4,5,7,8,2,3,6};
        std::vector<size_t> returned2{3,7,1,2,4,6,5,8};
        std::cout << GeneralizedLevensteinDistance(target,returned1) << std::endl;
        std::cout << GeneralizedLevensteinDistance(target,returned2) << std::endl;

    } else {
        std::string matrix = "matrix5.txt";
        std::string query = "caba";
        double tuning_factor = 0.0001;
        double lambda = 0.7;
        double min_path = std::numeric_limits<double>::epsilon()*2;
        size_t max_path = 5;

        std::cout << "MATRIX5" << std::endl;
        std::cout << "~~~~~~~" << std::endl;
        std::cout << "~~~~~~~" << std::endl;
        std::cout << "~~~~~~~" << std::endl;
        doBenchmark("matrix5.txt", query, tuning_factor, lambda, min_path, max_path);
        std::cout << "MATRIX6" << std::endl;
        std::cout << "~~~~~~~" << std::endl;
        std::cout << "~~~~~~~" << std::endl;
        std::cout << "~~~~~~~" << std::endl;
        doBenchmark("matrix6.txt", query, tuning_factor, lambda, min_path, max_path);


    } exit(1);
#endif

#if goodBenchmark
    ///parse();
    {
        ProposedRanking ir{-1, false, 200, 0.000001, 1.0};
        ir.loadGraph({"test2.txt", true, 1.0, input_format::REGEX});
        std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << ir.generateRanking("abcdefghijkl", 10) << std::endl;
    }
    {
        ProposedRanking ir{-1, false, 200, 0.000001, 1.0};
        ir.loadGraph({"test2.txt", true, 1.0, input_format::REGEX});
        std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << ir.generateRanking("GH", 10) << std::endl;
    }
    {
        ProposedRanking ir{-1, false, 200, 0.000001, 1.0};
        ir.loadGraph({"test2.txt", true, 1.0, input_format::REGEX});
        std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << ir.generateRanking("GHIJ", 10) << std::endl;
    }
#else // testingone

#endif

    ///testsDemultiplexed();


#if 0
    std::set<std::pair<std::string,std::string>> embedding_space;
    GraphEmbeddingStrategy*         ges  = new TransitiveClosureGraphStrategy<NodesWithTransitiveEdgeCost>(1.0, 100);

    ReadGraph g = ReadGraph::fromString("abc", 1.0);
    ReadGraph g1 = ReadGraph::fromString("abbaba", 1.0);
    ReadGraph g2 = ReadGraph::fromString("abc", 0.7);
    ReadGraph g3 = ReadGraph::fromString("abc", 0.2);

    const ReadGraph::unstructured_embedding &uv1 = (*ges)(g1);
    const ReadGraph::unstructured_embedding &uv2 = (*ges)(g2);
    const ReadGraph::unstructured_embedding &uv3 = (*ges)(g3);
    const ReadGraph::unstructured_embedding &uvg = (*ges)(g);
    ReadGraph::extractEmbeddingSpace(embedding_space, uv1);
    ReadGraph::extractEmbeddingSpace(embedding_space, uv2);
    ReadGraph::extractEmbeddingSpace(embedding_space, uv3);
    ReadGraph::extractEmbeddingSpace(embedding_space, uvg);

    auto v1 = ReadGraph::generateStructuredEmbedding(embedding_space, uv1);
    auto v2 = ReadGraph::generateStructuredEmbedding(embedding_space, uv2);
    auto v3 = ReadGraph::generateStructuredEmbedding(embedding_space, uv3);
    auto t = ReadGraph::generateStructuredEmbedding(embedding_space, uvg);

    std::cout << v1.format(CleanFmt) << std::endl;
    std::cout << t.format(CleanFmt) << std::endl;

    std::cout << score(v1,t) << " vs. " << scoreNormalized(v1, t) << std::endl;
    std::cout << score(v2,t) << " vs. " << scoreNormalized(v2, t) << std::endl;
    std::cout << score(v3,t) << " vs. " << scoreNormalized(v3, t) << std::endl;
    std::cout << score(t,t) << " vs. " << scoreNormalized(t, t) << std::endl;
    std::cout << score(v3,v3) << " vs. " << scoreNormalized(v3, v3) << std::endl;

#endif
}

#include "ConfigurationFile.h"
#include <utils/xml_utils.h>
#include <QtWidgets/QApplication>
#include <gui/WSettings.h>
#include <args.hxx>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {


    args::ArgumentParser parser("FuzzyStringMatching (2) (c) 2020-2021 by Giacomo Bergami.", "This free and open software program implements the (Approximate) Probabilistic Trace Alignment. Youse at your own risk.");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::Group group(parser, "You can use the following parameters", args::Group::Validators::DontCare, args::Options::Global);
    args::Flag gui(group, "gui", "Shows the GUI for setting the specified configuration file (when no parameter is specified, this is the default behaviour). The gui will store the resulting configuration file file once the window is closed.", {'g', "gui"});
    args::Flag run(group, "run", "Runs the program accordingly to the configuration file. If both --gui and --run are set, first the GUI is displayed, and then the benchmark is run", {'r', "run"});
    args::ValueFlag<std::string> con(group, "configuration.yaml", "Specifies the configuration file to edit (--gui) or to use to run the program (--run). If no configuration file is specified, the program will look for 'configuration.yaml'. If that file is not provided, a default configuration will be set-up. Run --gui to see the file", {'c', "conf"});
    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help& ) {
        std::cout << parser;
        return 0;
    } catch (args::ParseError& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    } catch (args::ValidationError& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    std::string yaml = "configuration.yaml";
    if (con) {
        yaml = args::get(con);
    }

    if (gui && run) {
        pid_t child = fork();
        if (child) {
            wait(nullptr); // Unfortunately, by quitting the window, the process will terminate. Therefore, I fork a child process for the gui, I wait for it, and then I run the benchmarks
        } else {
            QApplication app(argc, (char**)argv);
            WSettings window{yaml};
            window.show();
            return app.exec();
        }
    }

    if (run) {
        ConfigurationFile conf{yaml};
        conf.run();
    } else if (gui) {
        QApplication app(argc, (char**)argv);
        WSettings window{yaml};
        window.show();
        return app.exec();
    } else {
        std::cout << parser;
    }

}

void
doBenchmark(const std::string &matrix, const std::string &query, double tuning_factor, double lambda, double min_path,
            size_t max_path) {
    Ranking<std::string> expected = generate_example_expected_ranking(matrix, query);

    std::cout << std::endl << std::endl << std::endl << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    MultiplePathsEmbeddingStrategy* mpes = new EmbedPathsOverSingleGraphStrategy<NodesWithTransitiveEdgeCost>(tuning_factor, lambda, false, max_path, min_path);
    GraphEmbeddingStrategy*         ges  = new TransitiveClosureGraphStrategy<NodesWithTransitiveEdgeCost>(tuning_factor, lambda,  max_path);
    Ranking<std::string> competitor = doTestDemultiplexed2(1.0, mpes, ges, matrix, query);
    delete mpes;
    delete ges;

    LevensteinSimilarity similarity;
    std::cout << "Ranking distance: " << expected.normalizedRank(competitor, [&similarity](const std::string& left, const std::string& right) { return similarity.similarity(left, right); })  << std::endl;
    std::cout << "Spearman Index: " << expected.SpearmanCorrelationIndex(competitor, 1.0) << std::endl;
    std::cout << "Derived Ranking: " << minimum_edit_maximum_substring(competitor.reorderForInducedRanking(expected)) << std::endl;
    std::cout << "expected" << std::endl <<  expected << std::endl;
    std::cout << "competitor" << std::endl << competitor << std::endl;
}
