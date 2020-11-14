//
// Created by giacomo on 14/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_SortingEuclideanVectors_H
#define FUZZYSTRINGMATCHING2_SortingEuclideanVectors_H


#include <topk/Ladenprogramm.h>
#include <topk/topk.h>

/**
 * Sorts the vectors according to the distance from the origin. This definition is ideal for the vectors in the transformed space
 * Therefore, the query vector is always ignore, and always assumed to be the origin of the axes in the transformed space
 *
 */
class SortingTransformedSpaceVectors : public Ladenprogramm {
    std::vector<trivial_vector_record> trivial_storage;

public:
    SortingTransformedSpaceVectors() : Ladenprogramm() {}
    virtual ~SortingTransformedSpaceVectors() override {}

    void clear() override;

    virtual Ladenprogramm *setSpaceDimensionWithMove(size_t dimension) override;
    virtual size_t pushNovelBulkInsertedData() override;
    virtual TopKRanking topK(const std::vector<double> &query, size_t k) override;
    int supportedStrategy() override { return UnterstuetzenStrategie::TransformedSpace; }

protected:
    virtual void
    storeInMatrix(size_t i, const std::vector<double> &cps, const std::pair<size_t, size_t> &graph_trace_id) override;
    std::function<double(double, double)> pow2Norm = [](const auto& l, const auto& r) { return l+(r*r); };
};


#endif //FUZZYSTRINGMATCHING2_SortingEuclideanVectors_H
