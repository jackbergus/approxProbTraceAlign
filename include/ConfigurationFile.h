//
// Created by giacomo on 11/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_INPUTFORMAT_H
#define FUZZYSTRINGMATCHING2_INPUTFORMAT_H

#include <string>
#include <distances/WeightEstimator.h>
#include <chrono>
#include <iostream>
#include <vector>
#include <ReadGraph.h>

enum FileFormat {
    Petri_PNML,
    Petri_BPMN,
    PetriMatrix,
    StochasticMatrix,
    ProbRegex
};

enum TracesFormat {
    XESLog,
    RawLog,
    NoLog
};

enum LogOperations {
    LogSample,
    LogFilterFrequency,
    LogFilterValues
};

struct LogOperationConfiguration {
    LogOperations operation;
    double        factor;                // sampling size
    bool          keep_low_up_otherwise;

    LogOperationConfiguration() {}
    LogOperationConfiguration(const LogOperationConfiguration&) = default;
    LogOperationConfiguration& operator=(const LogOperationConfiguration&) = default;
    LogOperationConfiguration(LogOperations operation, double factor, bool keepLowUpOtherwise) : operation(operation),
                                                                                                 factor(factor),
                                                                                                 keep_low_up_otherwise(
                                                                                                         keepLowUpOtherwise) {}
};

#include <utils/fixed_bimap.h>

struct ConfigurationFile {
    /**
     * Loading the class from a configuration file, if it exists. Otherwise, the default parameters are going to be loaded
     * @param filename
     */
    ConfigurationFile(const std::string& filename);

    char  varepsilon = '.';
    std::string  admissibleCharList{"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"};

    ReadGraph    finalGraph;
    FileFormat   input_file_format      = PetriMatrix;
    std::string  input_file             = "data/petri_matrix.txt";
    bool         is_input_compressed    = false;
    size_t       ith_graph              = 0;

    TracesFormat trace_file_format              = TracesFormat::NoLog;
    std::string  traces_file;
    bool         are_traces_compressed          = false;
    char         separator_if_any               = ';';
    std::vector<LogOperationConfiguration> operations;
    std::unordered_set<std::string>               finalLog;

    bool         add_traces_to_log              = true;
    size_t       max_length                     = std::numeric_limits<size_t>::max();
    double       min_prob                       = std::numeric_limits<double>::epsilon()*2;

    bool                         use_estimator;
    spd_we::WeightEstimatorCases estimator_type = spd_we::W_CONSTANT;

    fixed_bimap<std::string, char>   action_to_single_char;

    void run();
    void serialize();

private:
    std::string configuration_filename;
};

bool isFileFormatPetri(enum FileFormat format);

#endif //FUZZYSTRINGMATCHING2_INPUTFORMAT_H