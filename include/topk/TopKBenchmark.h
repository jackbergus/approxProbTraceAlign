//
// Created by giacomo on 13/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_TOPKBENCHMARK_H
#define FUZZYSTRINGMATCHING2_TOPKBENCHMARK_H


#include "Ladenprogramm.h"
#include "Aufgabenstellung.h"
#include <ReadGraph.h>
#include <vector>
#include "basics.h"
#include <log/log_operations.h>


class TopKBenchmark {
    Ladenprogramm* laden;
    Aufgabenstellung* auf;
    EinbettungRaum raum;
    TopKBenchmarkDataset* dataset;
    bool sameDataset;
    bool singleGraphEmbedding;
    path_info prevQuery;

    void loadData();

public:
    TopKBenchmark(Ladenprogramm *laden, Aufgabenstellung *auf, bool singleGraphEmbedding = false);

    void clearAndReload(TopKBenchmarkDataset* dataset, const path_info &new_query);
    TopKRanking topK(const path_info& new_query, size_t k);
};



#endif //FUZZYSTRINGMATCHING2_TOPKBENCHMARK_H
