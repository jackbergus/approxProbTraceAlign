//
// Created by giacomo on 13/11/20.
//

#include "topk/TopKBenchmark.h"

TopKBenchmark::TopKBenchmark(Ladenprogramm *laden, Aufgabenstellung *auf, bool singleGraphEmbedding)
        : laden(laden), auf(auf), singleGraphEmbedding(singleGraphEmbedding) {
    assert(laden);// check to pass the correct representations
    assert(auf);  // check to pass the correct representation
    assert(laden->supportedStrategy() & (int)auf->getStrategy()); // check that the two wrappers are compatible and have the same embedding assumptions
}

void TopKBenchmark::clearAndReload(TopKBenchmarkDataset *dataset, const path_info &new_query) {
    if ((this->dataset != dataset) || (auf->requireDataReloadingAtEachQuery() && (new_query != prevQuery))) {
        sameDataset = false;
        auf->clear();
        raum.clear();
        this->dataset = dataset;
        this->prevQuery = new_query;
    } else {
        sameDataset = true;
    }
    loadData(); // clearing laden in here!
}

void TopKBenchmark::loadData() {
    if (sameDataset && auf->requireDataReloadingAtEachQuery()) {
        size_t i = 0;
        Ladenprogramm* cpy = laden->setSpaceDimensionWithMove(auf->getEmbeddingSpace(raum, dataset));
        if (cpy != laden) { // If now I have a copy
            delete laden; laden = cpy;
        }
        laden->clear();

        for (auto& cp: *dataset) {
            ReadGraph& graph_i = cp.first;
            std::vector<path_info>& log_i = cp.second;

            if (singleGraphEmbedding) {
                laden->bulkInsertGraph(i, auf->graphSelfEmbedding(graph_i, prevQuery));
            } else {
                size_t j = 0;
                for (const auto& trace_j : log_i) {
                    laden->bulkInsertGraph(i, j, auf->traceEmbedding(trace_j, graph_i, prevQuery));
                    j++;
                }
            }

            i++;
        }
    }
}

TopKRanking TopKBenchmark::topK(const path_info &query, size_t k) {
    return laden->topK(auf->queryEmbedding(query), k);
}
