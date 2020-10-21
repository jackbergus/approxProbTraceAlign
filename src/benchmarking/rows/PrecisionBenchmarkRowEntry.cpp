//
// Created by giacomo on 20/10/20.
//

#include <magic_enum.hpp>
#include "benchmarking/rows/PrecisionBenchmarkRowEntry.h"

PrecisionBenchmarkRowEntry::PrecisionBenchmarkRowEntry(const std::string &datasetName, const std::string &configuration,
                                                       size_t querySize, size_t distinctQueryChars,
                                                       case_precision_type eSCase, double expectedScore) : dataset_name(
        datasetName), configuration(configuration), query_size(querySize), distinct_query_chars(distinctQueryChars),
                                                                                                           eSCase(eSCase),
                                                                                                           expectedScore(
                                                                                                                   expectedScore) {}

fort::char_table &PrecisionBenchmarkRowEntry::init_header(fort::char_table &os) {
    return os << "dataset_name" << "configuration" << "query_size" << "distinct_query_chars" << "eSCase" << "expectedScore" << fort::endr;
}

std::ostream &operator<<(std::ostream &os, const PrecisionBenchmarkRowEntry &entry) {
    return os << entry.dataset_name << ',' << entry.configuration <<',' <<entry.query_size<<','<<entry.distinct_query_chars<<','<<magic_enum::enum_name(entry.eSCase) << ','<< entry.expectedScore << std::endl;
}

fort::char_table &operator<<(fort::char_table &os, const PrecisionBenchmarkRowEntry &entry) {
    return os << entry.dataset_name << entry.configuration <<entry.query_size<<entry.distinct_query_chars<<magic_enum::enum_name(entry.eSCase) << entry.expectedScore << fort::endr;

}
