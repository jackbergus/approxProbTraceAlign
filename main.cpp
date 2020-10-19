


#include "ReadGraph.h"
#include "RegexVisitor.h"

double score(const Eigen::VectorXd& left, const Eigen::VectorXd& right) {
    return left.dot(right);
}
double scoreNormalized(const Eigen::VectorXd& left, const Eigen::VectorXd& right) {
    return left.dot(right) / std::sqrt(left.dot(left) * right.dot(right));
}

Eigen::IOFormat CleanFmt(4, 0, "", ",", "", "", "[", "]");

void doTest(double lambda, double stringWeight) {
    std::cout << "lambda = " << lambda << " stringWeight = " << stringWeight << std::endl;
    std::set<std::pair<std::string,std::string>> embedding_space;
    ReadGraph t{"matrix.txt"};
    t.generateDecomposedEmbeddings(lambda);
    auto map = t.generateEmbeddings(embedding_space);


    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

    ReadGraph g = ReadGraph::fromString("abc", stringWeight);
    g.generateEmbeddings(lambda); // Do not use the decay factor
    g.pushGraphEmbedding();
    for (const auto& x :
            g.generateEmbeddings(embedding_space)) {
        for (const auto& y : map) {
            std::cout << "abc vs. " << y.first << " = " << score(x.second, y.second) << ", normalized = " << scoreNormalized(x.second, y.second) << std::endl;
        }
    }
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
}

void doMultiplePetriTest(double lambda, double stringWeight) {

    std::cout << "lambda = " << lambda << " stringWeight = " << stringWeight << std::endl;
    std::set<std::pair<std::string,std::string>> embedding_space;

    ReadGraph g1{"matrix.txt"};
    g1.generateEmbeddings(lambda);
    g1.pushGraphEmbedding();
    g1.extractEmbeddingSpace(embedding_space);

    ReadGraph g2{"matrix2.txt"};
    g2.generateEmbeddings(lambda);
    g2.pushGraphEmbedding();
    g2.extractEmbeddingSpace(embedding_space);

    ReadGraph g3{"matrix3.txt"};
    g3.generateEmbeddings(lambda);
    g3.pushGraphEmbedding();
    g3.extractEmbeddingSpace(embedding_space);

    ReadGraph g = ReadGraph::fromString("abc", stringWeight);
    g.generateEmbeddings(lambda); // Do not use the decay factor
    g.pushGraphEmbedding();

    auto v1 = g1.generateEmbeddings(embedding_space).find("*")->second;
    auto v2 = g2.generateEmbeddings(embedding_space).find("*")->second;
    auto v3 = g3.generateEmbeddings(embedding_space).find("*")->second;
    auto t = g.generateEmbeddings(embedding_space).find("*")->second;

    std::cout << "abc vs. matrix1  = " << score(v1, t) << ", normalized = " << scoreNormalized(v1, t) << std::endl;
    std::cout << "abc vs. matrix2  = " << score(v2, t) << ", normalized = " << scoreNormalized(v2, t) << std::endl;
    std::cout << "abc vs. matrix3  = " << score(v3, t) << ", normalized = " << scoreNormalized(v3, t) << std::endl;


    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

}

void tests() {

    doTest(1.0, 1.0);
    doTest(1.0, 0.3);
    doTest(0.7, 1.0);
    doTest(0.7, 0.3);

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
    //regexLexer* lexer = new regexLexer(&stream);
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

int main() {

    ///parse();
    ReadGraph t{"matrix4.txt"};
    for (const auto& x : t.iterateOverPaths(false, 10)) {
        std::cout << x << std::endl;
    }
    /*while (x.hasNext()) {
        std::cout << x.next() << std::endl;
    }*/
}
