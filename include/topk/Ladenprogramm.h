//
// Created by giacomo on 13/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_LADENPROGRAMM_H
#define FUZZYSTRINGMATCHING2_LADENPROGRAMM_H


#include <vector>
#include <utils/pair_hash.h>
#include <benchmarking/Ranking.h>
#include <topk/Ladenprogramm.h>
#include <unordered_map>
#include <faiss/IndexFlat.h>
#include <faiss/IndexIVFFlat.h>

using TopKRanking = Ranking<std::pair<size_t, size_t>>;

namespace std {
    template<> struct hash<std::pair<size_t, size_t>> {
        pair_hash ph;
        size_t operator() (const std::pair<size_t, size_t>& cp) const {
            return ph(cp);
        }
    };
}

struct Ladenprogramm {
    size_t totalGraphs = 0, totalTraces = 0, vectorsToLoad = 0;
    std::unordered_map<size_t, std::vector<double>> datasetGraph;
    std::unordered_map<std::pair<size_t, size_t>, std::vector<double>, pair_hash> datasetTrace;

    Ladenprogramm();
    virtual ~Ladenprogramm();
    virtual void setDatasetSize(size_t totalGraphs, size_t totalTraces, size_t vectorsToLoad);
    virtual void clear();
    virtual Ladenprogramm* setSpaceDimensionWithMove(size_t dimension) = 0;
    virtual void bulkInsertGraph(size_t graphId, const std::vector<double> &embedding);
    virtual void bulkInsertGraph(size_t graphId, size_t traceId, const std::vector<double> &embedding);
    virtual size_t pushNovelBulkInsertedData();
    virtual TopKRanking topK(const std::vector<double>& query, size_t k) = 0;
    virtual int supportedStrategy() { return 0; }

protected:

    virtual void storeInMatrix(size_t i, const std::vector<double> &cps, const std::pair<size_t, size_t> &graph_trace_id) = 0;
    size_t bulkLoad();
};

#endif //FUZZYSTRINGMATCHING2_LADENPROGRAMM_H
