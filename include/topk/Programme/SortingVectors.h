//
// Created by giacomo on 14/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_SortingEuclideanVectors_H
#define FUZZYSTRINGMATCHING2_SortingEuclideanVectors_H


#include <topk/Ladenprogramm.h>
#include <topk/topk.h>

/**
 * Partial sorting the vectors according to a n-dimensional query and k.
 * This has no assumptions on the space, and just performs the norm distance with the other vector
 *
 */
class SortingVectors : public Ladenprogramm {
    std::vector<trivial_vector_record> trivial_storage;

public:
    SortingVectors() : Ladenprogramm() {}
    virtual ~SortingVectors() override {}

    void clear() override;

    Ladenprogramm *setSpaceDimensionWithMove(size_t dimension) override;
    size_t pushNovelBulkInsertedData() override;
    TopKRanking topK(const std::vector<double> &query, size_t k) override;
    int supportedStrategy() override { return UnterstuetzenStrategie::EuclideanSpace; }

protected:
    void
    storeInMatrix(size_t i, const std::vector<double> &cps, const std::pair<size_t, size_t> &graph_trace_id) override;
    std::function<double(double, double)> vecdiffPow2Norm = [](const double& left, const double& right){ double xkc = (left-right); return xkc*xkc; };
    std::function<double(const std::vector<double>&, const std::vector<double>&)> vecmp = [this](const std::vector<double>& x, const std::vector<double>& y)
            { return std::inner_product(x.begin(), x.end(), y.begin(), 0, std::plus<>(), vecdiffPow2Norm); };
};


#endif //FUZZYSTRINGMATCHING2_SortingEuclideanVectors_H
