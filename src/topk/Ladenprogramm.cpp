//
// Created by giacomo on 13/11/20.
//

#include <topk/Ladenprogramm.h>


Ladenprogramm::Ladenprogramm() {}

~Ladenprogramm::Ladenprogramm() {}

void Ladenprogramm::setDatasetSize(size_t totalGraphs, size_t totalTraces, size_t vectorsToLoad) {
    this->totalGraphs = totalGraphs;
    this->totalTraces = totalTraces;
    this->vectorsToLoad = vectorsToLoad;
}

void Ladenprogramm::clear() {
    setDatasetSize(0,0,0);
}

size_t Ladenprogramm::pushNovelBulkInsertedData() {
    assert((datasetGraph.empty() && (!datasetTrace.empty())) || (datasetTrace.empty() && (!datasetGraph.empty())));
    return datasetGraph.empty() ? datasetTrace.size() : datasetGraph.size();
}

void Ladenprogramm::bulkInsertGraph(size_t graphId, const std::vector<double> &embedding) {
    datasetGraph[graphId] = embedding;
}

void Ladenprogramm::bulkInsertGraph(size_t graphId, size_t traceId, const std::vector<double> &embedding) {
    datasetTrace[std::make_pair(graphId, traceId)] = embedding;
}

size_t Ladenprogramm::bulkLoad() {
    size_t i = 0;
    if (datasetGraph.empty()) {
        for (const auto& cp : datasetTrace) {
            auto& cps = cp.second;
            this->storeInMatrix(i, cps, cp.first);
            i++;
        }
    } else {
        for (const auto& cp: datasetGraph) {
            auto& cps = cp.second;
            this->storeInMatrix(i, cps, std::make_pair(cp.first, (size_t)-1));
            i++;
        }
    }
    return i;
}