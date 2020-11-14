//
// Created by giacomo on 19/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_THOMSONNFA_H
#define FUZZYSTRINGMATCHING2_THOMSONNFA_H

#include <lemon/list_graph.h>
#include "data_loading/regex/Regex.h"
#include "ReadGraph.h"
#define         EPSILON     (".")

class ThomsonNFA {
    using Node = lemon::ListDigraph::Node;
    lemon::ListDigraph g;
    lemon::ListDigraph::NodeMap<std::string> nodeLabels;
    lemon::ListDigraph::NodeMap<size_t> inDeg, outDeg, nodeId;
    lemon::ListDigraph::ArcMap<double>       edgeCost;
    Node source, target;
    std::vector<Node> epsNodes;

    /**
     * Adds an arc from source to target, while incrementing the counter of ingoing and outgoing edges. Plus, it also
     * updates the cost map
     *
     * @param src       Source node
     * @param dst       Target node
     * @param cost      Edge probability
     */
    void addArc(const lemon::ListDigraph::Node& src, const lemon::ListDigraph::Node& dst, double cost = 1.0);

    /**
     * Adds a new node that has empty label
     * @return
     */
    inline lemon::ListDigraphBase::Node addEpsilonNode();

    inline Node addNode(const std::string& label);

    /**
     * Generates subgraphs for given regexes
     * @param result        Source and targets from the previous iteration
     * @param doSkip        Whether the previous iteration could be skipped
     * @param doLoop        Whether the previous iteration could be looped
     * @param cost          The cost associated to each of the choices
     * @return
     */
    inline std::pair<lemon::ListDigraph::Node, lemon::ListDigraph::Node> generateForLoops(const  std::pair<lemon::ListDigraph::Node, lemon::ListDigraph::Node>& result, bool doSkip, bool doLoop, double cost = 1.0);


    /**
     * Given a regex, generates the subgraph and adds it to the current Thomson Graph
     *
     * @param element   Regex to be visited recursively to generate the graph
     *
     * @return Source and target nodes
     */
    std::pair<lemon::ListDigraph::Node, lemon::ListDigraph::Node> generate(Regex<std::string>* element);


    /**
     * Performs the epsilon closure, thus removing the epsilon nodes that are neither the source nor the target node,
     * that should have non-empty indegree and non-empty outdegree
     */
    bool epsilonClosure();

public:
    ThomsonNFA() :  nodeLabels{g}, edgeCost{g}, inDeg{g, 0}, outDeg{g, 0}, nodeId{g, 0} {}
    ThomsonNFA(Regex<std::string>* regex);
    ThomsonNFA(const ThomsonNFA&) = default;
    ThomsonNFA(ThomsonNFA&&) = default;
    ThomsonNFA& operator=(const ThomsonNFA&) = default;
    ThomsonNFA(FILE* file);

    /**
     * Loads a graph file as an NFA to be minimized.
     * @param filename
     */
    ThomsonNFA(const std::string& filename, bool isFile);


    /**
     * Converts the loaded DFA (after the epsilonClosure) into a ReadGraph, so we can run our algorithms over it!
     *
     * @param withCost    whole cost associated to the whole regex/nfa
     *
     * @return            Useful representation
     */
    ReadGraph generateGraph(double withCost = 1.0);

};


#endif //FUZZYSTRINGMATCHING2_THOMSONNFA_H
