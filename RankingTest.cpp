//
// Created by giacomo on 21/10/20.
//

#include "RankingTest.h"

double RankingTest::generateRanking(const std::string &query, size_t topK) {
    std::vector<ReadGraph> graphs;
    bangBang.fillVector(graphs);
    size_t pathId = 0;
    std::cerr << "Loading Path Embedding" << std::endl;
    for (ReadGraph& rg : graphs) {
        for (const struct path_info& path: rg.iterateOverPaths(doNotVisiPahtsTwive, maxPathLength, minimumPathCost) ) {
            std::cerr << " * Graph = " << rg.name << " path= " << path.path << std::endl;
            competitorRanking.addScore(pathId, calculateCompetitorAlignmentCost(path, pathId, rg, query));
            loadPathEmbedding(path, pathId, rg, query);
            pathId++;
        }
    }
    std::cerr << "Indexing" << std::endl;
    indexingEmbedding();
    std::cerr << "Ranking" << std::endl;
    auto generateTopKRank = generateRank(query, topK);
    auto project = generateTopKRank.topK(topK);
    auto projectCompetitor = competitorRanking.topK(topK);
    return project.SpearmanCorrelationIndex(projectCompetitor, 0.0);
}

RankingTest::RankingTest(bool doNotVisiPahtsTwive, size_t maxPathLength, double minimumPathCost) : doNotVisiPahtsTwive(
        doNotVisiPahtsTwive), maxPathLength(maxPathLength), minimumPathCost(minimumPathCost) {}
