//
// Created by giacomo on 21/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_GRAPHCARTRIDGE_H
#define FUZZYSTRINGMATCHING2_GRAPHCARTRIDGE_H

#if 0
// TODO: REMOVE
#include <InputStream.h>
#include <unordered_map>
#include <vector>

class GraphCartridge {
    InputStream stream;
    std::unordered_map<std::string, ReadGraph> graphs;

public:
    void loadGraph(const ReadGraphConfiguration &grafo);
    ReadGraph getGraph(const std::string& name) const;
    std::string getGraphCollectionName() const;
    void fillVector(std::vector<ReadGraph>& toFill) const;

};
#endif //FUZZYSTRINGMATCHING2_GRAPHCARTRIDGE_H
#endif