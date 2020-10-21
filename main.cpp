


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


#if 1
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
#endif
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



}



#include <fstream>
#include <regexLexer.h>
#include <regexParser.h>
#include <benchmarking/Ranking.h>
#include "Regex.h"
#include "ThomsonNFA.h"
#include "utils/AlterString.h"
#include "distances/path/GenericStringCostFunction.h"


void parse() {
    std::ifstream file("test.txt", std::ifstream::binary);
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
    ReadGraph t{"matrix4.txt"};
    for (const auto& x : t.iterateOverPaths(false, 20, 0.000001)) {
        std::cout << x << std::endl;
    }
}

#include <ostream>
#include <distances/strings/LevensteinDistance.h>
#include <benchmarking/BenchmarkStrategy.h>

struct BenchmarkGenericStringCostFunction : public BenchmarkStrategy {

    GenericStringCostFunction<LevensteinDistance> pathSimilarity{1, 1, 1};

    BenchmarkGenericStringCostFunction(const std::string &datasetName, const std::string &configuration,
                                       bool doNotVisitLoopsTwice, size_t maxPathLength, const double minimumPathCost)
            : BenchmarkStrategy(datasetName, configuration, doNotVisitLoopsTwice, maxPathLength, minimumPathCost,
                                [](const ReadGraphRankingStruct& left, const ReadGraphRankingStruct& right) {
                                    return GeneralizedLevensteinDistance(left.readGraphPath, right.readGraphPath);
            }){}

    void scorePath(const std::string &query, ReadGraph *, const struct path_info &path_from_graph,
                   PathRanking &pathRanking, double &benchmark_time) override {
        auto start = std::chrono::system_clock::now();
        double score = pathSimilarity.distance(query, path_from_graph);
        auto end = std::chrono::system_clock::now();
        pathRanking.addScore(path_from_graph, score);
        benchmark_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
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
        benchmark_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }

};

int main() {



}
