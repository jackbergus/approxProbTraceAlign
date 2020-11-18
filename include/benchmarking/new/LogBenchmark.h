//
// Created by giacomo on 17/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_LOGBENCHMARK_H
#define FUZZYSTRINGMATCHING2_LOGBENCHMARK_H

#include <ostream>
#include <vector>
#include <ConfigurationFile.h>
#include <magic_enum.hpp>

struct LogBenchmark {
    static void log_record_element_per_query(std::ostream &file);
    static void log_record_element_per_log(std::ostream &file);
    static void log_record_element_per_query(std::ostream& file, const std::vector<record_element_per_query>& log);
    static void log_record_element_per_log(std::ostream& file, const std::vector<additional_benchmarks_per_log>& log);
};


#endif //FUZZYSTRINGMATCHING2_LOGBENCHMARK_H
