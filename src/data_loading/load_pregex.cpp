//
// Created by giacomo on 13/11/20.
//

#include "data_loading/load_pregex.h"
#include "data_loading/regex/Regex.h"

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
#include <data_loading/regex/RegexVisitor.h>

GenericGraph<size_t> load_pregex(const std::string &filename, const std::string &epsilon) {

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
