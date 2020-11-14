//
// Created by giacomo on 14/11/20.
//

#include "topk/Programme/SortingTransformedSpaceVectors.h"

Ladenprogramm *SortingTransformedSpaceVectors::setSpaceDimensionWithMove(size_t dimension) {
    return this;
}

size_t SortingTransformedSpaceVectors::pushNovelBulkInsertedData() {
    size_t N = Ladenprogramm::pushNovelBulkInsertedData(); // assertion
    assert(bulkLoad() == N);
    std::sort(trivial_storage.begin(), trivial_storage.end(), [this](const auto& x, const auto& y){
        return std::accumulate(x.vector.begin(), x.vector.end(), 0, pow2Norm) <
                std::accumulate(y.vector.begin(), y.vector.end(), 0, pow2Norm);
    });
}

TopKRanking SortingTransformedSpaceVectors::topK(const std::vector<double> &query, size_t k) {
    for (const auto& arg: query) assert(arg == 0.0);
    TopKRanking toReturn;
    auto it = trivial_storage.rbegin();
    for (size_t i = 0; (i<k) && it != trivial_storage.rend(); i++) {
        toReturn.score(it->graph_and_trace_id, i+1);
        it++;
    }
    return toReturn;
}

void SortingTransformedSpaceVectors::storeInMatrix(size_t i, const std::vector<double> &cps,
                                                   const std::pair<size_t, size_t> &graph_trace_id) {
    trivial_storage.emplace_back(graph_trace_id, cps);
}

void SortingTransformedSpaceVectors::clear() {
    Ladenprogramm::clear();
    trivial_storage.clear();
}
