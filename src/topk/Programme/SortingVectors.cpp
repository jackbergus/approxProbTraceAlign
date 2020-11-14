//
// Created by giacomo on 14/11/20.
//

#include "topk/Programme/SortingVectors.h"

Ladenprogramm *SortingVectors::setSpaceDimensionWithMove(size_t dimension) {
    return this;
}

size_t SortingVectors::pushNovelBulkInsertedData() {
    size_t N = Ladenprogramm::pushNovelBulkInsertedData(); // assertion
    assert(bulkLoad() == N);
}

#include <algorithm>

TopKRanking SortingVectors::topK(const std::vector<double> &query, size_t k) {
    std::nth_element(trivial_storage.begin(), trivial_storage.begin() +k, trivial_storage.end(), [query, this](const auto& x, const auto& y){
        return vecmp(x.vector, query) < vecmp(y.vector, query);
    });
    TopKRanking toReturn;
    auto it = trivial_storage.rbegin();
    for (size_t i = 0; (i<k) && it != trivial_storage.rend(); i++) {
        toReturn.score(it->graph_and_trace_id, vecmp(it->vector, query));
        it++;
    }
    return toReturn;
}

void SortingVectors::storeInMatrix(size_t i, const std::vector<double> &cps,
                                                   const std::pair<size_t, size_t> &graph_trace_id) {
    trivial_storage.emplace_back(graph_trace_id, cps);
}

void SortingVectors::clear() {
    Ladenprogramm::clear();
    trivial_storage.clear();
}
