//
// Created by giacomo on 13/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_AUFGABENSTELLUNG_H
#define FUZZYSTRINGMATCHING2_AUFGABENSTELLUNG_H

#include <ReadGraph.h>
#include <string>
#include <topk/basics.h>

class Aufgabenstellung {
protected:
    EinbettungRaum* embeddingSpace;

public:
    Aufgabenstellung();
    virtual ~Aufgabenstellung();

    virtual size_t getEmbeddingSpace(EinbettungRaum* es, TopKBenchmarkDataset* data) = 0;
    virtual void clear();
    virtual std::vector<double> graphSelfEmbedding(ReadGraph &graph, const struct path_info &query) = 0;
    virtual std::vector<double>
    traceEmbedding(const struct path_info &pathInGraph, ReadGraph &graph, const struct path_info &query) = 0;
    virtual std::vector<double> queryEmbedding(const struct path_info& query) = 0;
    virtual bool requireDataReloadingAtEachQuery() { return true; }
};


#endif //FUZZYSTRINGMATCHING2_AUFGABENSTELLUNG_H
