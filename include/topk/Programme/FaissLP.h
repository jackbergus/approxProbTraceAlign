//
// Created by giacomo on 14/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_FAISSLP_H
#define FUZZYSTRINGMATCHING2_FAISSLP_H

#include <topk/Ladenprogramm.h>
#include <unordered_map>
#include <faiss/IndexFlat.h>
#include <faiss/IndexIVFFlat.h>
#include <topk/topk.h>

/**
 * Uses Facebook's FAISS to sort the vectors using a nearest neighbour technique
 */
class FaissLP : public Ladenprogramm {
    float *xb, *xq;
    size_t d;
    size_t nlist, nprobe;
    std::vector<std::pair<size_t, size_t>> convertCoordinates;
    faiss::IndexFlatL2 quantizer;
    faiss::IndexIVFFlat index;
    UnterstuetzenStrategie exitStrategy;

public:
    FaissLP(size_t nlist, size_t tentativeDimension, size_t nprobe, UnterstuetzenStrategie strategy);

    ~FaissLP() override;

    void clear() override;

    Ladenprogramm*  setSpaceDimensionWithMove(size_t dimension) override;
    size_t pushNovelBulkInsertedData() override;
    TopKRanking topK(const std::vector<double> &query, size_t k) override;
    void storeInMatrix(size_t i, const std::vector<double> &cps, const std::pair<size_t, size_t> &graph_trace_id) override ;
    int supportedStrategy() override {
        return exitStrategy;
    }

};


#endif //FUZZYSTRINGMATCHING2_FAISSLP_H
