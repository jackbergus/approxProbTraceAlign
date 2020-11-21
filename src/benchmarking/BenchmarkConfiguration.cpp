//
// Created by giacomo on 21/10/20.
//

#if 0
#include <utils/AlterString.h>
#include "benchmarking/BenchmarkConfiguration.h"

void BenchmarkConfiguration::loadGraph(const ReadGraphConfiguration &grafo) {
    graphCartridge.loadGraph(grafo);
}

BenchmarkConfiguration::BenchmarkConfiguration(BenchmarkStrategy *benchmark) : benchmark(benchmark) {}

std::string BenchmarkConfiguration::getGraphCollectionName() const {
    return graphCartridge.getGraphCollectionName();
}

void BenchmarkConfiguration::benchmarkMultiplePathsFromSameGraphWithNoise(const std::string &graphname, double maxNoise) {
    ReadGraph g = graphCartridge.getGraph(graphname);
    benchmark->configuration = "benchmarkGraphsPathsAgainstGraphWithNoise = " + std::to_string(maxNoise);
    benchmark->dataset_name = graphname;
    std::string seed{"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    AlterString as{seed, maxNoise};
    for (struct path_info pathCopy : g.iterateOverPaths(benchmark->doNotVisitLoopsTwice, benchmark->maxPathLength, benchmark->minimumPathCost)) {
        std::string query = as.alter(pathCopy.path);
        pathCopy.probability = 1.0 / (GeneralizedLevensteinDistance(query, pathCopy.path) / 5.0 + 1.0);
        benchmark->singleQueryBenchmark({query, pathCopy}, g);
    }
}

void BenchmarkConfiguration::benchmarkSinglePathAgainstGraph(const BenchmarkSeed &seed, const std::string &graphname) {
    benchmark->configuration = "benchmarkSinglePathAgainstGraph";
    benchmark->dataset_name = graphname;
    ReadGraph g = graphCartridge.getGraph(graphname);
    benchmark->singleQueryBenchmark(seed, g);
}

void BenchmarkConfiguration::benchmarkSinglePathAgainstGraphWithNoise(const BenchmarkSeed &seed,
                                                                      const std::string &graphname, double maxNoise,
                                                                      size_t iterations) {
    ReadGraph g = graphCartridge.getGraph(graphname);
    benchmark->configuration = "benchmarkGraphsPathsAgainstGraphWithNoise = " + std::to_string(maxNoise) + " forIterations= " + std::to_string(iterations);
    benchmark->dataset_name = graphname;
    std::string alterStringSeed{"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    AlterString as{alterStringSeed, maxNoise};
    for (size_t i = 0; i<iterations; i++) {
        BenchmarkSeed newSeed = seed;
        newSeed.query = as.alter(seed.query);
        newSeed.expected_result.probability = seed.expected_result.probability * (1.0 / (1.0 + GeneralizedLevensteinDistance(newSeed.query , seed.query) / 5.0));
        benchmark->singleQueryBenchmark(newSeed, g);
    }
}

void BenchmarkConfiguration::benchmarkSinglePathAgainstGraphCollection(const BenchmarkSeed &seed) {
    benchmark->configuration = "benchmarkSinglePathAgainstGraphCollection";
    benchmark->dataset_name = getGraphCollectionName();
    std::vector<ReadGraph> rg;
    graphCartridge.fillVector(rg);
    benchmark->singleQueryMultipleGraphsBenchmark(seed, rg);
}

void BenchmarkConfiguration::benchmarkMultiplePathsFromSameGraph(const std::string &graphname) {
    ReadGraph g = graphCartridge.getGraph(graphname);
    benchmark->configuration = "benchmarkGraphsPathsAgainstGraph";
    benchmark->dataset_name = graphname;
    for (struct path_info pathCopy : g.iterateOverPaths(benchmark->doNotVisitLoopsTwice, benchmark->maxPathLength, benchmark->minimumPathCost)) {
        pathCopy.probability = 1.0;
        benchmark->singleQueryBenchmark({pathCopy.path, pathCopy}, g);
    }
}

std::ostream &operator<<(std::ostream &os, const BenchmarkConfiguration &configuration) {
    return os << *configuration.benchmark;
}
#endif