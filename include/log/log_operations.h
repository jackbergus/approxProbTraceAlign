/*
 * log_operations.h
 * This file is part of ProbabilisticTraceAlignment
 *
 * Copyright (C) 2020 - Giacomo Bergami
 *
 * ProbabilisticTraceAlignment is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * ProbabilisticTraceAlignment is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ProbabilisticTraceAlignment. If not, see <http://www.gnu.org/licenses/>.
 */


//
// Created by giacomo on 13/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_LOG_OPERATIONS_H
#define FUZZYSTRINGMATCHING2_LOG_OPERATIONS_H

#include <log/LogOperationConfiguration.h>
#include <fptree.hpp>
#include <data_loading/local_load_data.h>

using LogType = Transaction<std::string>;

void performLogOperation(const std::vector<LogOperationConfiguration> &vconf,
                         std::vector<Transaction<TimestampedEvent>> &log);

#endif //FUZZYSTRINGMATCHING2_LOG_OPERATIONS_H
