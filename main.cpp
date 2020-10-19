


#include "ReadGraph.h"
#include "RegexVisitor.h"

double score(const Eigen::VectorXd& left, const Eigen::VectorXd& right) {
    return left.dot(right);
}
double scoreNormalized(const Eigen::VectorXd& left, const Eigen::VectorXd& right) {
    return left.dot(right) / std::sqrt(left.dot(left) * right.dot(right));
}

Eigen::IOFormat CleanFmt(4, 0, "", ",", "", "", "[", "]");


void getSingleGraphEmbedding(const std::string& name, double lambda, std::set<std::pair<std::string, std::string>> &embedding_space, ReadGraph &g1,
                             ReadGraph::path_to_uembedding &map1) {
    map1[name] = {g1.generateWholeGraphEmbedding(lambda)};
    ReadGraph::extractEmbeddingSpace(embedding_space, map1);
}

void getPathEmbeddings(double lambda, bool doNotVisitLoopsTwice, size_t maxPathLength, const double minimumPathCost,
                       std::set<std::pair<std::string, std::string>> &embedding_space, ReadGraph::path_to_uembedding &map,
                       ReadGraph &g1) {
    ConditionalReadGraphIterable it = g1.iterateOverPaths(doNotVisitLoopsTwice, maxPathLength, minimumPathCost);
    g1.generatePathEmbeddings(lambda, it, map);
    ReadGraph::extractEmbeddingSpace(embedding_space, map);
}


void doTest(double lambda, double stringWeight, bool doNotVisitLoopsTwice, size_t maxPathLength, const double minimumPathCost) {
    std::cout << "lambda = " << lambda << " stringWeight = " << stringWeight << std::endl;
    std::set<std::pair<std::string,std::string>> embedding_space;
    ReadGraph t{"matrix.txt"};
    ReadGraph::path_to_uembedding ptu, ptg;
    ConditionalReadGraphIterable it = t.iterateOverPaths(doNotVisitLoopsTwice, maxPathLength, minimumPathCost);
    t.generatePathEmbeddings(lambda, it, ptu);
    auto map = ReadGraph::generateStructuredEmbeddings(embedding_space, ptu);


    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

    ReadGraph g = ReadGraph::fromString("abc", stringWeight);
    ptg["g"] = {g.generateWholeGraphEmbedding(lambda)}; // Do not use the decay factor
    for (const auto& x : ReadGraph::generateStructuredEmbeddings(embedding_space, ptg)) {
        for (const auto& y : map) {
            std::cout << "\tabc vs. " << y.first << " = " << score(x.second, y.second) << ", normalized = " << scoreNormalized(x.second, y.second) << std::endl;
        }
    }
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
}



void doMultiplePetriTest(double lambda, double stringWeight) {
    std::cout << "lambda = " << lambda << " stringWeight = " << stringWeight << std::endl;
    std::set<std::pair<std::string,std::string>> embedding_space;
    ReadGraph::path_to_uembedding map1, map2, map3, mapg;

    ReadGraph g1{"matrix.txt"};
    getSingleGraphEmbedding("matrix.txt", lambda, embedding_space, g1, map1);

    ReadGraph g2{"matrix2.txt"};
    getSingleGraphEmbedding("matrix2.txt", lambda, embedding_space, g2, map2);

    ReadGraph g3{"matrix3.txt"};
    getSingleGraphEmbedding("matrix3.txt", lambda, embedding_space, g3, map3);

    ReadGraph g = ReadGraph::fromString("abc", stringWeight);
    getSingleGraphEmbedding("g", lambda, embedding_space, g, mapg);

    auto v1 = ReadGraph::generateStructuredEmbeddings(embedding_space, map1).find("matrix.txt")->second;//generateWholeGraphEmbedding
    auto v2 = ReadGraph::generateStructuredEmbeddings(embedding_space, map2).find("matrix2.txt")->second;//generateWholeGraphEmbedding
    auto v3 = ReadGraph::generateStructuredEmbeddings(embedding_space, map3).find("matrix3.txt")->second;//generateWholeGraphEmbedding
    auto t = ReadGraph::generateStructuredEmbeddings(embedding_space, mapg).find("g")->second;//generateWholeGraphEmbedding

    std::cout << "abc vs. matrix1  = " << score(v1, t) << ", normalized = " << scoreNormalized(v1, t) << std::endl;
    std::cout << "abc vs. matrix2  = " << score(v2, t) << ", normalized = " << scoreNormalized(v2, t) << std::endl;
    std::cout << "abc vs. matrix3  = " << score(v3, t) << ", normalized = " << scoreNormalized(v3, t) << std::endl;


    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

}





void tests() {
    doTest(1.0, 1.0, true, 100, 0.00001);
    doTest(1.0, 0.3, true, 100, 0.00001);
    doTest(0.7, 1.0, true, 100, 0.00001);
    doTest(0.7, 0.3, true, 100, 0.00001);

    doMultiplePetriTest(1.0, 1.0);
    doMultiplePetriTest(1.0, 0.3);
    doMultiplePetriTest(0.7, 1.0);
    doMultiplePetriTest(0.7, 0.3);
}

#include <fstream>
#include <regexLexer.h>
#include <regexParser.h>
#include "Regex.h"
#include "ThomsonNFA.h"
#include "AlterString.h"


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

int main() {

    tests();


}
