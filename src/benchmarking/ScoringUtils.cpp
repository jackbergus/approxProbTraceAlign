//
// Created by giacomo on 17/11/20.
//

#include "benchmarking/new/ScoringUtils.h"

Ranking<size_t> doDulcior(Ranking<size_t> &orig, size_t N) {
    Ranking<size_t> finalRanking;
    for (size_t i = 0; i<N; i++) {
        finalRanking.addScore(i, orig.getRanking(i));
    }
    return finalRanking;
}

double score(const Eigen::VectorXd& left, const Eigen::VectorXd& right) {
    return left.dot(right);
}
