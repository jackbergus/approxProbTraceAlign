//
// Created by giacomo on 13/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_LOGOPERATIONCONFIGURATION_H
#define FUZZYSTRINGMATCHING2_LOGOPERATIONCONFIGURATION_H

enum LogOperations {
    LogSample,
    LogFilterFrequency,
    LogFilterValues,
    LogFilterTemporalLength
};

struct LogOperationConfiguration {
    LogOperations operation;
    double        factor;                // sampling size
    bool          keep_low_up_otherwise;

    LogOperationConfiguration();
    LogOperationConfiguration(const LogOperationConfiguration&) = default;
    LogOperationConfiguration& operator=(const LogOperationConfiguration&) = default;
    LogOperationConfiguration(LogOperations operation, double factor, bool keepLowUpOtherwise);
};

#endif //FUZZYSTRINGMATCHING2_LOGOPERATIONCONFIGURATION_H
