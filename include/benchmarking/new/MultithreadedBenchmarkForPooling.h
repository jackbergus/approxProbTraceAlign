//
// Created by giacomo on 17/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_MULTITHREADEDBENCHMARKFORPOOLING_H
#define FUZZYSTRINGMATCHING2_MULTITHREADEDBENCHMARKFORPOOLING_H


#include <distances/strings/LevensteinSimilarity.h>
#include <embeddings/path_embedding/MultiplePathsEmbeddingStrategy.h>
#include <embeddings/graph_embedding/GraphEmbeddingStrategy.h>
#include <topk/Aussageform/DistanceExpressionEvaluator.h>
#include <magic_enum.hpp>
#include <set>
#include <embeddings/labelled_paths/NodesWithTransitiveEdgeCost.h>
#include <utils/AlterString.h>
#include <distances/strings/Levenstein.h>
#include <benchmarking/Ranking.h>
#include <benchmarking/new/minimum_edit_maximum_substring.h>

Ranking<size_t> doDulcior(Ranking<size_t> &orig, size_t N);

double score(const Eigen::VectorXd &left, const Eigen::VectorXd &right);

#endif //FUZZYSTRINGMATCHING2_MULTITHREADEDBENCHMARKFORPOOLING_H
