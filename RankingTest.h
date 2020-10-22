//
// Created by giacomo on 21/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_RANKINGTEST_H
#define FUZZYSTRINGMATCHING2_RANKINGTEST_H


#include "GraphCartridge.h"
#include <benchmarking/Ranking.h>
#include <matrix_graph_path/path_info.h>


class RankingTest {
    GraphCartridge bangBang;
    Ranking<size_t> competitorRanking;
    bool doNotVisiPahtsTwive;
    size_t maxPathLength;
    double minimumPathCost;


public:
    RankingTest(bool doNotVisiPahtsTwive, size_t maxPathLength, double minimumPathCost);
    ~RankingTest() {}
    void loadGraph(const ReadGraphConfiguration &grafo) { bangBang.loadGraph(grafo); }

    double generateRanking(const std::string& query, size_t topK);
    virtual void loadPathEmbedding(const struct path_info& path, size_t pathId, ReadGraph& sourceGraph, const std::string& query) = 0;
    virtual void indexingEmbedding()  = 0;
    virtual Ranking<size_t> generateRank(const std::string& query, size_t topK) = 0;
    double calculateCompetitorAlignmentCost(const struct path_info& path, size_t pathId, ReadGraph& sourceGraph, const std::string& query) {
        double cost = path.cost/(GeneralizedLevensteinDistance(path.path, query)/5.0+1.0);
        return cost;
    }

};


#endif //FUZZYSTRINGMATCHING2_RANKINGTEST_H