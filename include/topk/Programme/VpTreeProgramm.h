//
// Created by giacomo on 14/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_VPTREEPROGRAMM_H
#define FUZZYSTRINGMATCHING2_VPTREEPROGRAMM_H

#if 0
#include <data_structures/vptree.h>
#include <topk/Ladenprogramm.h>
#include <topk/topk.h>
#include <utils/vector_hash.h>

class VpTreeProgramm : public Ladenprogramm {
    vpt::VpTree<T* tree = nullptr;
    std::unordered_map<std::vector<double>, std::vector<std::pair<size_t, size_t>>, vector_hash<double>> trivial_storage;

    vpt::Metric ProbSim{[](const vpt::Vector& v1, const vpt::Vector& v2)  {
        return
                std::accumulate(v1.begin(), v2.end(), 1.0, std::multiplies<>()) -
                        std::accumulate(v2.begin(), v2.end(), 1.0, std::multiplies<>());
    }, ProbabilitySimilarity};

    vpt::Metric TransfSpace{[](const vpt::Vector& v1, const vpt::Vector& v2)  {
        return
                std::accumulate(v1.begin(), v2.end(), 1, [](double acc, double x) { return acc + (x*x);}) -
                std::accumulate(v2.begin(), v2.end(), 1, [](double acc, double x) { return acc + (x*x);});
    }, TransformedSpace};

    vpt::Metric EuclideanMetric{[](const vpt::Vector& v1, const vpt::Vector& v2)  {
        return std::inner_product(v1.begin(), v1.end(), v2.begin(), 0.0, std::plus<>(), [](double x, double y) {return (x-y)*(x-y); });
    }, EuclideanSpace};

    UnterstuetzenStrategie us;

public:
    VpTreeProgramm(UnterstuetzenStrategie us) : Ladenprogramm{}, us{us} {}
    ~VpTreeProgramm() override {}

    void clear() override;
    Ladenprogramm *setSpaceDimensionWithMove(size_t dimension) override;
    size_t pushNovelBulkInsertedData() override;
    TopKRanking topK(const std::vector<double> &query, size_t k) override;
    int supportedStrategy() override;

protected:
    void
    storeInMatrix(size_t i, const std::vector<double> &cps, const std::pair<size_t, size_t> &graph_trace_id) override;
};
#endif

#endif //FUZZYSTRINGMATCHING2_VPTREEPROGRAMM_H
