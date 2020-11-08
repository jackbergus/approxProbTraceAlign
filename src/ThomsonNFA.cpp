//
// Created by giacomo on 19/10/20.
//

#include "ThomsonNFA.h"

void ThomsonNFA::addArc(const lemon::ListDigraph::Node &src, const lemon::ListDigraph::Node &dst, double cost) {
    edgeCost[g.addArc(src, dst)] = cost;
    inDeg[dst]++;
    outDeg[src]++;
}

lemon::ListDigraphBase::Node ThomsonNFA::addEpsilonNode() {
    /*auto left = g.addNode();
    epsNodes.emplace_back(left);
    nodeLabels[left] = EPSILON;*/
    return addNode(EPSILON);
}

std::pair<lemon::ListDigraph::Node, lemon::ListDigraph::Node>
ThomsonNFA::generateForLoops(const std::pair<lemon::ListDigraph::Node, lemon::ListDigraph::Node> &result, bool doSkip,
                             bool doLoop, double cost) {
    assert(std::abs(cost) <= 1.0);
    auto left  = addEpsilonNode();
    auto right = addEpsilonNode();
    double rightCost = 1.0;
    double leftCost  = 1.0;

    if (doLoop) {
        addArc(result.second, result.first, cost);
        rightCost = 1.0-cost;
    }

    if (doSkip) {
        addArc(left, right, cost);
        leftCost = 1.0-cost;
    }

    addArc(left,          result.first, leftCost );
    addArc(result.second, right,        rightCost);
    return {left, right};
}

std::pair<lemon::ListDigraph::Node, lemon::ListDigraph::Node> ThomsonNFA::generate(Regex<std::string> *element) {
    assert(element);
    switch (element->cases()) {
        case Consequent:
        {
            auto left = generate(element->getLeft());
            auto right = generate(element->getRight());
            edgeCost[g.addArc(left.second, right.first)] = element->getCost();
            return {left.first, right.second};
        }

        case BinaryAlternative:
        {
            const double cost = element->getCost();
            assert(std::abs(cost) <= 1.0);
            auto left = generate(element->getLeft());
            auto right = generate(element->getRight());

            auto src =  addEpsilonNode();
            addArc(src, left.first, cost);
            addArc(src, right.first, 1.0-cost);

            auto dst =  addEpsilonNode();
            addArc(left.second, dst);
            addArc(right.second, dst);

            return {src, dst};
        }
            break;
        case Plus:
            return generateForLoops(generate(element->getLeft()), false, true, element->getCost());
        case Star:
            return generateForLoops(generate(element->getLeft()), true, true, element->getCost());
        case Optional:
            return generateForLoops(generate(element->getLeft()), true, false, element->getCost());
        case Paren:
            return generate(element->getLeft());
        case String: {
            auto x = g.addNode();
            nodeLabels[x] = element->getLabel();
            return {x,x};
        }
        case NONE:
            assert(false);
    }
}

bool ThomsonNFA::epsilonClosure() {
    bool someChange = false;
    auto it = epsNodes.begin();
    while(it != epsNodes.end()) {
        if (*it == source) {
            std::vector<std::pair<Node, double>> outs, ins;
            for (lemon::ListDigraph::OutArcIt a(g, *it); a != lemon::INVALID; ++a) {
                outs.emplace_back(g.target(a), edgeCost[a]);
            }
            for (lemon::ListDigraph::InArcIt a(g, *it); a != lemon::INVALID; ++a) {
                ins.emplace_back(g.source(a), edgeCost[a]);
            }
            if (ins.empty() && (outs.size() == 1)) {
                assert(outs[0].second);
                source = outs[0].first;
                g.erase(*it);
                it = epsNodes.erase(it);
            } else {
                it++;
            }
        } else if (*it == target) {
            std::vector<std::pair<Node, double>> outs, ins;
            for (lemon::ListDigraph::OutArcIt a(g, *it); a != lemon::INVALID; ++a) {
                outs.emplace_back(g.target(a), edgeCost[a]);
            }
            for (lemon::ListDigraph::InArcIt a(g, *it); a != lemon::INVALID; ++a) {
                ins.emplace_back(g.source(a), edgeCost[a]);
            }
            if (outs.empty() && (ins.size() == 1)) {
                assert(ins[0].second);
                target = ins[0].first;
                g.erase(*it);
                it = epsNodes.erase(it);
            } else {
                it++;
            }
        } else {
            std::vector<std::pair<Node, double>> outs, ins;
            for (lemon::ListDigraph::OutArcIt a(g, *it); a != lemon::INVALID; ++a) {
                outs.emplace_back(g.target(a), edgeCost[a]);
            }
            for (lemon::ListDigraph::InArcIt a(g, *it); a != lemon::INVALID; ++a) {
                ins.emplace_back(g.source(a), edgeCost[a]);
            }
            if ((!outs.empty()) && (!ins.empty())) {
                someChange = true;
                g.erase(*it);
                it = epsNodes.erase(it);
                for (const auto& x : ins) {
                    for (const auto& y: outs) {
                        addArc(x.first, y.first, x.second*y.second);
                    }
                }
            } else ++it;
        }
    }
    return someChange;
}

ThomsonNFA::ThomsonNFA(Regex<std::string> *regex) : ThomsonNFA{} {
    std::tie(source, target) = generate(regex); // determines which are the source and the target node for the graphs
    while (epsilonClosure()); // Performs the epsilon closure from the graph, so
}

ReadGraph ThomsonNFA::generateGraph(double withCost) {
    size_t edges = 0, nodes = 0;
    ReadGraph ret;
    for (auto it = g.arcs().begin(); it != g.arcs().end(); it++) edges++;
    for (auto it = g.nodes().begin(); it != g.nodes().end(); it++) {
        size_t id = nodes++;
        nodeId[*it] = id;
    }
    ret.init(nodes, edges, nodeId[source], nodeId[target]);
    for (auto it = g.nodes().begin(); it != g.nodes().end(); it++) {
        ret.addNode(nodeId[*it], nodeLabels[*it]);
    }
    for (auto it = g.arcs().begin(); it != g.arcs().end(); it++) {
        ret.addEdge(nodeId[g.source(*it)], nodeId[g.target(*it)], edgeCost[*it]);
    }
    ret.finalizeEdgesMatrix(withCost);
    ///ret.printGraph();
    return ret;
}

ThomsonNFA::Node ThomsonNFA::addNode(const std::string &label) {
    auto left = g.addNode();
    if (label == EPSILON)
        epsNodes.emplace_back(left);
    nodeLabels[left] = label;
    return left;
}

ThomsonNFA::ThomsonNFA(const std::string &filename, bool isFile) : ThomsonNFA{} {
    std::unordered_map<size_t, Node> toNode;
    FILE *file;
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
        if (error) return;
        i = fscanf(file, "edges: %zd\n", &edges);
        error = (i == EOF || (i != 1));
        if (error) return;
        i = fscanf(file, "source: %zd\n", &source);
        error = (i == EOF || (i != 1));
        if (error) return;
        i = fscanf(file, "target: %zd\n", &target);
        error = (i == EOF || (i != 1));
        if (error) return;
        i = fscanf(file, "weight: %lf\n", &w);
        error = (i == EOF || (i != 1));
        if (error) return;

        for (size_t j = 0; j<nodes; j++) {
            size_t node_no;
            char string[124];
            std::string k;
            i = fscanf(file, "%zd %123s\n", &node_no, string);
            error = (i == EOF || (i != 2));
            if (error) return;
            k = std::string(string);
            toNode.emplace(node_no, addNode(k));
        }


        for (size_t j = 0; j<edges; j++) {
            size_t src, dst;
            double weight;
            i = fscanf(file, "%zd %zd %lf\n", &src, &dst, &weight);
            error = (i == EOF || (i != 3));
            if (error) return;
            assert(std::abs(weight) <= 1.0);
            addArc(toNode.at(src), toNode.at(dst), weight);
        }
        fclose(file);
        while (epsilonClosure());
    }
}

ThomsonNFA::ThomsonNFA(FILE *file) : ThomsonNFA{} {
    std::unordered_map<size_t, Node> toNode;
    size_t error = 0;
    size_t nodes = 0, edges = 0;
    if (file) {
        int i;
        double w = 1;
        // Reading the number of the nodes
        i = fscanf(file, "nodes: %zd\n", &nodes);
        error = (i == EOF || (i != 1));
        if (error) return;
        i = fscanf(file, "edges: %zd\n", &edges);
        error = (i == EOF || (i != 1));
        if (error) return;
        i = fscanf(file, "source: %zd\n", &source);
        error = (i == EOF || (i != 1));
        if (error) return;
        i = fscanf(file, "target: %zd\n", &target);
        error = (i == EOF || (i != 1));
        if (error) return;
        i = fscanf(file, "weight: %lf\n", &w);
        error = (i == EOF || (i != 1));
        if (error) return;

        for (size_t j = 0; j<nodes; j++) {
            size_t node_no;
            char string[124];
            std::string k;
            i = fscanf(file, "%zd %123s\n", &node_no, string);
            error = (i == EOF || (i != 2));
            if (error) return;
            k = std::string(string);
            toNode.emplace(node_no, addNode(k));
        }


        for (size_t j = 0; j<edges; j++) {
            size_t src, dst;
            double weight;
            i = fscanf(file, "%zd %zd %lf\n", &src, &dst, &weight);
            error = (i == EOF || (i != 3));
            if (error) return;
            assert(std::abs(weight) <= 1.0);
            addArc(toNode.at(src), toNode.at(dst), weight);
        }
        fclose(file);
        while (epsilonClosure());
    }
}
