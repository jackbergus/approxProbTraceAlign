//
// Created by giacomo on 20/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_TIMEBENCHMARKROWENTRY_H
#define FUZZYSTRINGMATCHING2_TIMEBENCHMARKROWENTRY_H


enum case_time_benchmark {
    PATH_SIZE,
    N_DISTINCT_NODES,
    TOTAL_TIME,
    AVG_TIME
};

#include <ostream>
#include <magic_enum.hpp>

std::ostream& operator <<(std::ostream &o, const enum case_time_benchmark& p);

#include <string>
#include <fort.hpp>

struct TimeBenchmarkRowEntry {
    std::string          dataset;
    std::string          experiment_type;
    enum case_time_benchmark  entry_type;
    double               query_length;
    double               query_no_distinct_nodes;
    double               entry_value;
    double               time;

    TimeBenchmarkRowEntry(const std::string &dataset, const std::string &experimentType, case_time_benchmark entryType,
                          double queryLength, double queryNoDistinctNodes, double entryValue, double time);

    friend std::ostream &operator<<(std::ostream &os, const TimeBenchmarkRowEntry &entry);//fort::char_table
    friend fort::char_table &operator<<(fort::char_table &os, const TimeBenchmarkRowEntry &entry);
    static fort::char_table & init_header(fort::char_table &os);
};


#endif //FUZZYSTRINGMATCHING2_TIMEBENCHMARKROWENTRY_H
