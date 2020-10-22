//
// Created by giacomo on 21/10/20.
//

#include "GraphCartridge.h"

void GraphCartridge::loadGraph(const ReadGraphConfiguration &grafo) {
    ReadGraph g = stream.readGraph(grafo);
    g.printGraph();
    assert(graphs.insert(std::make_pair(g.name, g)).second);
    //graphVector.emplace_back(g);
}

ReadGraph GraphCartridge::getGraph(const std::string &name) const {
    return graphs.at(name);
}

std::string GraphCartridge::getGraphCollectionName() const {
    std::stringstream iss;
    iss << "{";
    for (auto it = graphs.begin(), en = graphs.end(); it != en; ) {
        iss << it->first;
        it++;
        if (it != en) {
            iss << ", ";
        }
    }
    iss << '}';
    return iss.str();
}

void GraphCartridge::fillVector(std::vector<ReadGraph> &toFill) const {
    toFill.clear();
    for (const auto& cp : graphs) toFill.emplace_back(cp.second);
}
