//
// Created by giacomo on 14/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_GEGENWAERTIGA_H
#define FUZZYSTRINGMATCHING2_GEGENWAERTIGA_H

#include <topk/Aufgabenstellung.h>
#include <embeddings/path_embedding/MultiplePathsEmbeddingStrategy.h>
#include <embeddings/graph_embedding/GraphEmbeddingStrategy.h>
#include <distances/strings/GenericStringSimilarity.h>

class GegenwaertigA : public Aufgabenstellung {
    UnterstuetzenStrategie s;
    MultiplePathsEmbeddingStrategy *strategy;
    GraphEmbeddingStrategy *testStrategy;
    GenericStringSimilarity *stringSimilarity;
    std::function<std::vector<double>(double, double)> vectorTransformation;
    size_t Mpath;
    double mProb;
    size_t embeddingSize;
    std::vector<double> traceEmbeddingWorstCase;
    std::vector<double> psWorst{-1.0, -1.0};
    std::vector<double> trWorst{std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};

public:
    GegenwaertigA(UnterstuetzenStrategie x, MultiplePathsEmbeddingStrategy *strategy,
                  GraphEmbeddingStrategy *testStrategy, GenericStringSimilarity *stringSimilarity,
                  std::function<std::vector<double>(double, double)> vectorTransformation,
                  size_t graphSelfEmbeddingMaxPathLength, double graphSelfEmbeddingMinPathProb);

    ~GegenwaertigA() override {}

    size_t getEmbeddingSpace(EinbettungRaum &es, TopKBenchmarkDataset *data) override;

    void clear() override {
        Aufgabenstellung::clear();
    }

private:
    using ggfLambda =  std::function<std::vector<double>(const struct path_info &, ReadGraph &,
                                                         const struct path_info &)>;
    ggfLambda probabilitySimilarityLambda =
            [this](const struct path_info &path, ReadGraph &graph, const struct path_info &query) {
                return psVector(path, query);
            };
    ggfLambda transformedSpaceLambda =
            [this](const struct path_info &path, ReadGraph &graph, const struct path_info &query) {
                auto x = psVector(path, query);
                return vectorTransformation(x[0], x[1]);
            };
    ggfLambda euclideanSpaceLambda =
            [this](const struct path_info &path, ReadGraph &graph,
                   const struct path_info &) { return getVectorFromPathAndStrategy(path, graph); };

public:
    std::vector<double> graphSelfEmbedding(ReadGraph &graph, const struct path_info &query) override;


    std::vector<double>
    traceEmbedding(const struct path_info &pathInGraph, ReadGraph &graph, const struct path_info &query) override;

    std::vector<double> queryEmbedding(const struct path_info &query) override;

    bool requireDataReloadingAtEachQuery() const override { return s != EuclideanSpace; }

    UnterstuetzenStrategie getStrategy() const override { return s; }

private:
    // utility methods

    std::vector<double>
    psVector(const path_info &pathInGraph, const path_info &query) const {
        return {pathInGraph.cost * query.cost, stringSimilarity->similarity(pathInGraph.path, query.path)};
    }

    std::vector<double> getVectorFromPathAndStrategy(const path_info &pathInGraph, ReadGraph &graph);

    std::vector<double> getVectorFromGraphAndStrategy(ReadGraph &pathGraph);

    std::vector<double> genericGenerationFunction(ReadGraph &graph, const struct path_info &query, ggfLambda &rec,
                                                  const std::vector<double> &worstCase);
};


#endif //FUZZYSTRINGMATCHING2_GEGENWAERTIGA_H
