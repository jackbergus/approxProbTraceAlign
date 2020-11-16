//
// Created by giacomo on 16/11/20.
//

#include "topk/old/functor_rankings.h"
#include <topk/Aussageform/ExpressionEvaluator.h>>

TopKRanking generateExampleExpectedRanking(size_t graph_i, std::unordered_map<std::pair<size_t, size_t>, struct path_info, pair_hash> &pathsInGraph,
                                           const std::string &caba, bool dulcior, ExpressionEvaluator *probSimMetric) {
    std::map<double, std::vector<std::pair<size_t, size_t>>> rankingMap;
    assert(probSimMetric && (probSimMetric->getStrategy() == UnterstuetzenStrategie::ProbabilitySimilarity));
    TopKRanking expectedRanking;
    for (const auto& x : pathsInGraph) {
        double similarity = 1.0/(GeneralizedLevensteinDistance(x.second.path, caba)/5.0+1.0);
        double finalScore = probSimMetric->operator()({x.second.probability, similarity});
        expectedRanking.addScore(x.first, finalScore);
        ///std::cout << x.second << ": probability=" << x.first << " distance: " << distance << " similarity: " << score << " RANKING: " << x.first * score << std::endl;
        if (!dulcior) rankingMap[finalScore].emplace_back(x.first);
    }
    for (const auto& x : rankingMap) assert(x.second.size() == 1); // Imposing that there are no elements with the same scoring value
    if (!dulcior) {
        return expectedRanking;
    } else {
        TopKRanking finalRanking;
        for (const auto& y: pathsInGraph) {
            finalRanking.addScore(y.first, expectedRanking.getRanking(y.first));
        }
        return finalRanking;
    }
    //size_t maxRank = rankingMap.size();
    /* maxRank = 1; for (auto it = rankingMap.rbegin(); it != rankingMap.rend(); it++) {
        std::cout << maxRank << " [" << it->first << "]" << std::endl;
        for (const auto & y : it->second) {
            std::cout << '\t' << y << " ";
        }
        std::cout << std::endl;
        maxRank++;
    }*/

}
