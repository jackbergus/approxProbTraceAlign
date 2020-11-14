//
// Created by giacomo on 14/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_SORTINGPROBABILITYSIMILARITYVECTORS_H
#define FUZZYSTRINGMATCHING2_SORTINGPROBABILITYSIMILARITYVECTORS_H


#include <topk/Ladenprogramm.h>
#include <topk/topk.h>

/**
 * Using the trivial vector data storage to store the vectors, and sorting the vectors by the product of their components (ideally, the vectors have only two dimensions)
 * This technique is just for vectors <p,s>
 *
 */
class SortingProbabilitySimilarityVectors : public Ladenprogramm {
    std::vector<trivial_vector_record> trivial_storage;

public:
    SortingProbabilitySimilarityVectors() : Ladenprogramm() {}
    virtual ~SortingProbabilitySimilarityVectors() override {}

    void clear() override;
    Ladenprogramm *setSpaceDimensionWithMove(size_t dimension) override;
    size_t pushNovelBulkInsertedData() override;
    TopKRanking topK(const std::vector<double> &query, size_t k) override;
    int supportedStrategy() override { return UntersuetzenStrategie::ProbabilitySimilarity; }

protected:
    void
    storeInMatrix(size_t i, const std::vector<double> &cps, const std::pair<size_t, size_t> &graph_trace_id) override;

};


#endif //FUZZYSTRINGMATCHING2_SORTINGPROBABILITYSIMILARITYVECTORS_H
