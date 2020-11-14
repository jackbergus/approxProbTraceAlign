//
// Created by giacomo on 14/11/20.
//

#include <topk/Programme/FaissLP.h>

FaissLP::FaissLP(size_t nlist, size_t tentativeDimension, size_t nprobe, UntersuetzenStrategie strategy) :
    Ladenprogramm{}, nlist{nlist}, nprobe{nprobe}, xb{nullptr}, xq{nullptr}, d{tentativeDimension},
    quantizer{static_cast<long>(tentativeDimension)}, index{&quantizer, d, nlist}, exitStrategy{strategy} {
    assert(exitStrategy != UntersuetzenStrategie::ProbabilitySimilarity);
}

FaissLP::~FaissLP() {
    delete xb; xb = nullptr;
    delete xq; xq = nullptr;
    d = 0;
    datasetGraph.clear();
    datasetTrace.clear();
    convertCoordinates.clear();
}

void FaissLP::clear() {
    delete xb; xb = nullptr;
    delete xq; xq = nullptr;
    d = 0;
    datasetGraph.clear();
    datasetTrace.clear();
    convertCoordinates.clear();
}

size_t FaissLP::pushNovelBulkInsertedData() {
    size_t N = Ladenprogramm::pushNovelBulkInsertedData(); // assertion
    delete xb; xb = nullptr; xb = new float[d * N];
    convertCoordinates.clear();
    assert(bulkLoad() == N); // updating convertCoordinates via indirect call to the overridden storeInMatrix

    quantizer.reset();
    index.reset();
    index.train(N, xb);
    index.add(N, xb);
}

void FaissLP::storeInMatrix(size_t i, const std::vector<double> &cps, const std::pair<size_t, size_t> &graph_trace_id) {
    assert(cps.size() == d);
    convertCoordinates.emplace_back(graph_trace_id);
    for (size_t j = 0, M = cps.size(); j < M; j++) {
        xb[d * i + j] = (float)cps[j];
    }
}

TopKRanking FaissLP::topK(const std::vector<double> &query, size_t k) {
    if (exitStrategy != UntersuetzenStrategie::EuclideanSpace) {
        for (const auto& arg : query) if (exitStrategy == UntersuetzenStrategie::TransformedSpace) assert(arg == 0.0); else assert(arg == 1.0);
    }
    TopKRanking toInstruct;
    delete xq; xq = nullptr; xq = new float[d];

    if (exitStrategy == TransformedSpace) {
        memset((void*)xq, 0, sizeof(float)*d);
    } else {
        for(int j = 0; j < d; j++)
            xq[j] = (float)query[j];
    }

    {
        faiss::Index::idx_t *I = new faiss::Index::idx_t[k];
        float *D = new float[k];

        index.search(1, xq, k, D, I);

        index.nprobe = 10;
        index.search(1, xq, k, D, I);

        for (size_t i = 0; i<k; i++) {
            toInstruct.score(convertCoordinates[I[i]], D[i]);
        }

        delete [] I;
        delete [] D;
    }

    return toInstruct;
}

Ladenprogramm *FaissLP::setSpaceDimensionWithMove(size_t dimension) {
    if (d != dimension) {
        FaissLP* toRet = new FaissLP(nlist, dimension, nprobe, exitStrategy);
        toRet->xb = xb; toRet->xq = xq;
        std::swap(toRet->datasetGraph, datasetGraph);
        std::swap(toRet->datasetTrace, datasetTrace);
        std::swap(toRet->convertCoordinates, convertCoordinates);
        return toRet;
    } else {
        return this;
    }
}

