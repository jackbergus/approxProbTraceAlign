//
// Created by giacomo on 14/11/20.
//

#include "topk/Programme/VpTreeProgramm.h"

void VpTreeProgramm::clear() {
    Ladenprogramm::clear();
    delete tree; tree = nullptr;
    trivial_storage.clear();
}

Ladenprogramm *VpTreeProgramm::setSpaceDimensionWithMove(size_t dimension) {
    return this;
}

size_t VpTreeProgramm::pushNovelBulkInsertedData() {
    delete tree; tree = nullptr;
    trivial_storage.clear();
    size_t N = Ladenprogramm::pushNovelBulkInsertedData(); // assertion
    assert(bulkLoad() == N);
}

TopKRanking VpTreeProgramm::topK(const std::vector<double> &query, size_t k) {
    if (us != UntersuetzenStrategie::EuclideanSpace) {
        for (const auto& arg : query) if (us == UntersuetzenStrategie::TransformedSpace) assert(arg == 0.0); else assert(arg == 1.0);
    }
    vpt::DistancesIndices results;
    std::vector<std::vector<double>> V{trivial_storage.size()};
    {
        for (const auto& v : trivial_storage) V.emplace_back(v.first);
        tree = new vpt::VpTree(V, (us == EuclideanSpace) ? EuclideanMetric : ((us == TransformedSpace) ? TransfSpace : ProbSim));
        results = tree->getNearestNeighbors(query, k);
    }

    TopKRanking result;
    for (size_t i = 0, n = results.first.size(); i<n; i++) {
        for (const auto& x : trivial_storage[V[results.second[i]]]) {
            result.score(x, results.first[i]);
        }
    }
    return result;
}

int VpTreeProgramm::supportedStrategy() {
    return tree ? tree->getDistance.strategy : 0;
}

void
VpTreeProgramm::storeInMatrix(size_t i, const std::vector<double> &cps, const std::pair<size_t, size_t> &graph_trace_id) {
    trivial_storage[cps].emplace_back(graph_trace_id);
}
