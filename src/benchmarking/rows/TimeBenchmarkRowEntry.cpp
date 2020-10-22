//
// Created by giacomo on 20/10/20.
//

#include <benchmarking/rows/TimeBenchmarkRowEntry.h>

std::ostream &operator<<(std::ostream &o, const enum case_time_benchmark &p) {
    return o << magic_enum::enum_name(p);
}

std::ostream &operator<<(std::ostream &os, const TimeBenchmarkRowEntry &entry) {
    return os << entry.dataset << "," << entry.experiment_type << "," << entry.query_length << ',' << entry.query_no_distinct_nodes << ',' << entry.entry_type << "," << entry.entry_value << "," << entry.time;
}

fort::char_table &operator<<(fort::char_table &os, const TimeBenchmarkRowEntry &entry) {
    return os << entry.dataset << entry.experiment_type << entry.query_length << entry.query_no_distinct_nodes << magic_enum::enum_name(entry.entry_type) << entry.entry_value << entry.time <<  fort::endr;
}

fort::char_table &TimeBenchmarkRowEntry::init_header(fort::char_table &os) {
    return os << fort::header << "dataset" << "experiment_type" << "query_length" << "query_no_distinct_nodes" << "entry_type" << "entry_value" << "time "  << fort::endr;
}

TimeBenchmarkRowEntry::TimeBenchmarkRowEntry(const std::string &dataset, const std::string &experimentType,
                                             case_time_benchmark entryType, double queryLength, double queryNoDistinctNodes,
                                             double entryValue, double time) : dataset(dataset),
                                                                       experiment_type(experimentType),
                                                                       entry_type(entryType), query_length(queryLength),
                                                                       query_no_distinct_nodes(queryNoDistinctNodes),
                                                                       entry_value(entryValue), time(time) {}
