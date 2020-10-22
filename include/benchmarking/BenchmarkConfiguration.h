//
// Created by giacomo on 21/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_BENCHMARKCONFIGURATION_H
#define FUZZYSTRINGMATCHING2_BENCHMARKCONFIGURATION_H


#include <InputStream.h>
#include <benchmarking/BenchmarkStrategy.h>
#include <ostream>
#include "../../GraphCartridge.h"

class BenchmarkConfiguration {
    GraphCartridge graphCartridge;
    BenchmarkStrategy* benchmark;

public:
    std::string getGraphCollectionName() const;
    void loadGraph(const ReadGraphConfiguration& grafo);
    BenchmarkConfiguration(BenchmarkStrategy *benchmark);

    void benchmarkSinglePathAgainstGraph(const BenchmarkSeed& seed, const std::string& graphname);
    void benchmarkSinglePathAgainstGraphWithNoise(const BenchmarkSeed& seed, const std::string& graphname, double maxNoise, size_t iterations);
    void benchmarkSinglePathAgainstGraphCollection(const BenchmarkSeed& seed);
    void benchmarkMultiplePathsFromSameGraph(const std::string& graphname);
    void benchmarkMultiplePathsFromSameGraphWithNoise(const std::string& graphname, double maxNoise);

    friend std::ostream &operator<<(std::ostream &os, const BenchmarkConfiguration &configuration);

};


#endif //FUZZYSTRINGMATCHING2_BENCHMARKCONFIGURATION_H
