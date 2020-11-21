/*
 * local_load_data.h
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
