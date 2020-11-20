//
// Created by giacomo on 13/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_LOCAL_LOAD_DATA_H
#define FUZZYSTRINGMATCHING2_LOCAL_LOAD_DATA_H


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

#include <string>

long long int parse8601(const std::string& save);

struct TimestampedEvent {
    std::string event_name;
    long long int millisSinceEpoch;

    TimestampedEvent(const std::string &eventName, long long int millisSinceEpoch);

    TimestampedEvent();
    TimestampedEvent(const TimestampedEvent&) = default;
    TimestampedEvent& operator=(const TimestampedEvent&) = default;

    bool operator==(const TimestampedEvent &rhs) const;
    bool operator!=(const TimestampedEvent &rhs) const;

    bool operator<(const TimestampedEvent &rhs) const;

    bool operator>(const TimestampedEvent &rhs) const;

    bool operator<=(const TimestampedEvent &rhs) const;

    bool operator>=(const TimestampedEvent &rhs) const;
};

bool isFileFormatPetri(enum FileFormat format);


#include <data_loading/load_xes.h>
#include <data_loading/load_pnml.h>
#include <data_loading/load_bpmn.h>
#include <data_loading/load_petri_matrix.h>
#include <data_loading/load_matrix.h>
#include <data_loading/load_pregex.h>

#endif //FUZZYSTRINGMATCHING2_LOCAL_LOAD_DATA_H
