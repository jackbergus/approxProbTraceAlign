//
// Created by giacomo on 13/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_LOG_OPERATIONS_H
#define FUZZYSTRINGMATCHING2_LOG_OPERATIONS_H

#include <log/LogOperationConfiguration.h>
#include <fptree.hpp>

void performLogOperation(const std::vector<LogOperationConfiguration> &vconf,
                         std::vector<Transaction<std::string>> &log);

#endif //FUZZYSTRINGMATCHING2_LOG_OPERATIONS_H
