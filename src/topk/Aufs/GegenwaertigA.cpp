//
// Created by giacomo on 14/11/20.
//

#include "topk/Aufs/GegenwaertigA.h"

GegenwaertigA::GegenwaertigA(UnterstuetzenStrategie x, MultiplePathsEmbeddingStrategy *strategy,
                             GraphEmbeddingStrategy *testStrategy, GenericStringSimilarity *stringSimilarity,
                             std::function<std::vector<double>(double, double)> vectorTransformation,
                             size_t graphSelfEmbeddingMaxPathLength, double graphSelfEmbeddingMinPathProb)
        : s{x}, strategy(strategy), testStrategy(testStrategy), stringSimilarity(stringSimilarity),
          Mpath(graphSelfEmbeddingMaxPathLength),
          mProb(graphSelfEmbeddingMinPathProb), vectorTransformation(vectorTransformation) {}

size_t GegenwaertigA::getEmbeddingSpace(EinbettungRaum &es, TopKBenchmarkDataset *data) {
    assert(data);
    if (s != EuclideanSpace) {
        embeddingSize = 2;
    } else {
        for (auto &cp : *data) {
            ReadGraph::extractEmbeddingSpace(raum, (*strategy)(cp.first));
            ReadGraph::extractEmbeddingSpace(raum, (*testStrategy)(cp.first));
        }
    }
    {
        std::vector<double> traceEmbeddingWorstCase(embeddingSize, -1);
        std::swap(this->traceEmbeddingWorstCase, traceEmbeddingWorstCase);
    }
    return embeddingSize;
}

std::vector<double> GegenwaertigA::graphSelfEmbedding(ReadGraph &graph, const struct path_info &query) {
    switch (s) {
        case ProbabilitySimilarity:
            std::cerr
                    << "Warning: cannot provide a self embedding for the ProbabilitySimilarity strategy, for which I would expect an alignment."
                    << std::endl;
            std::cerr
                    << "Therefore, I'm reducing the problem to finding the best trace alignment for the current query trace"
                    << std::endl;
            return genericGenerationFunction(graph, query, probabilitySimilarityLambda, psWorst);
        case TransformedSpace:
            std::cerr
                    << "Warning: cannot provide a self embedding for the TransformedSpace strategy, for which I would expect an alignment."
                    << std::endl;
            std::cerr
                    << "Therefore, I'm reducing the problem to finding the best trace alignment for the current query trace"
                    << std::endl;
            return genericGenerationFunction(graph, query, transformedSpaceLambda, trWorst);
        case EuclideanSpace:
            return getVectorFromGraphAndStrategy(graph);
    }
}

std::vector<double>
GegenwaertigA::traceEmbedding(const struct path_info &pathInGraph, ReadGraph &graph, const struct path_info &query) {
    switch (s) {
        case ProbabilitySimilarity:
            return psVector(pathInGraph, query);

        case TransformedSpace: {
            return vectorTransformation(pathInGraph.cost * query.cost,
                                        stringSimilarity->similarity(pathInGraph.path, query.path));
        }

        case EuclideanSpace: {
            if (!pathInGraph.actualPath.empty()) {
                // If the presented path does actually come from the graph of interest
                return getVectorFromPathAndStrategy(pathInGraph, graph);
            } else {
                std::cerr
                        << "WARNING: the current trace used to represent a decomposition of the graph does not necessarily belong to the graph itself. Therefore, I am approximating its representation to the graph via alignment"
                        << std::endl;
                std::cerr
                        << "The approximation uses the trivial solution: find all the best traces from the graph, and pick one that best approximates it!"
                        << std::endl;
                return genericGenerationFunction(graph, query, euclideanSpaceLambda, traceEmbeddingWorstCase);
                //std::vector<double> approx;
                /*double score = -1.0, newScore = -1.0;
                for (const auto& path : graph.iterateOverPaths(false, graphSelfEmbeddingMaxPathLength, graphSelfEmbeddingMinPathProb)) {
                    newScore = path.cost*query.cost* stringSimilarity->similarity(path.path, query.path);
                    if (newScore > score) {
                        approx = getVectorFromPathAndStrategy(path, graph);
                        score = newScore;
                    }
                }
                if (approx.empty()) {
                    std::cerr << "WARNING: no good approximation was found. I'm returning a negative error, that will have the worst score of them all" << std::endl;
                    approx.insert(approx.begin(), embeddingSize, -1);
                }
                return approx;*/

            }

        }
    }
}

std::vector<double> GegenwaertigA::queryEmbedding(const struct path_info &query) {
    switch (s) {
        case ProbabilitySimilarity:
            return {1.0, 1.0};
        case TransformedSpace:
            return {0.0, 0.0};

        case EuclideanSpace: {
            ReadGraph g = ReadGraph::fromString(query.path, query.cost);
            auto tmp = (*testStrategy)(g);
            auto x = ReadGraph::generateStructuredEmbedding(raum, tmp);
            return {x.data(), x.data() + x.rows() * x.cols()};
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<double> GegenwaertigA::getVectorFromPathAndStrategy(const path_info &pathInGraph, ReadGraph &graph) {
    ReadGraph pathGraph;
    double pathCost = ReadGraph::generateGraphFromPath(pathInGraph.actualPath, pathGraph, graph.inv_label_conversion,
                                                       graph.A);
    pathGraph.finalizeEdgesMatrix(graph.weight * pathCost);
    return getVectorFromGraphAndStrategy(pathGraph);
}

std::vector<double> GegenwaertigA::getVectorFromGraphAndStrategy(ReadGraph &pathGraph) {
    ReadGraph::unstructured_embedding ue = (*testStrategy)(pathGraph);
    auto x = ReadGraph::generateStructuredEmbedding(raum, ue);
    return {x.data(), x.data() + x.rows() * x.cols()};
}

std::vector<double>
GegenwaertigA::genericGenerationFunction(ReadGraph &graph, const struct path_info &query, GegenwaertigA::ggfLambda &rec,
                                         const std::vector<double> &worstCase) {
    std::vector<double> approx;
    double score = mProb, newScore = mProb;
    for (const auto &path : graph.iterateOverPaths(false, Mpath, mProb)) {
        newScore = path.cost * query.cost * stringSimilarity->similarity(path.path, query.path);
        if (newScore > score) {
            approx = rec(path, graph, query);
            score = newScore;
        }
    }
    if (score != mProb) {
        return approx;
    } else {
        std::cerr
                << "WARNING: no good approximation was found. I'm returning a negative error, that will have the worst score of them all"
                << std::endl;
        return worstCase;
    }
}
