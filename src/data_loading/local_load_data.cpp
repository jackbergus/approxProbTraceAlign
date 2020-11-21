/*
 * local_load_data.cpp
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
#include_next <data_loading/local_load_data.h>

bool isFileFormatPetri(enum FileFormat format) {
    switch (format) {

        case Petri_PNML:
        case Petri_BPMN:
        case PetriMatrix:
            return true;

        case StochasticMatrix:
        case ProbRegex:
        default:
            return false;
    }
}

#include <chrono>
#include <iomanip>
#include <date.h>

long long int parse8601(const std::string &save) {
    std::istringstream in{save};
    date::sys_time<std::chrono::milliseconds> tp;
    in >> date::parse("%FT%TZ", tp);
    if (in.fail())
    {
        in.clear();
        in.exceptions(std::ios::failbit);
        in.str(save);
        in >> date::parse("%FT%T%Ez", tp);
    }
    return std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
}

bool TimestampedEvent::operator==(const TimestampedEvent &rhs) const {
    return event_name == rhs.event_name &&
           millisSinceEpoch == rhs.millisSinceEpoch;
}

bool TimestampedEvent::operator!=(const TimestampedEvent &rhs) const {
    return !(rhs == *this);
}

TimestampedEvent::TimestampedEvent(const std::string &eventName, long long int millisSinceEpoch) : event_name(eventName),
                                                                                                   millisSinceEpoch(millisSinceEpoch) {}

TimestampedEvent::TimestampedEvent() : TimestampedEvent{"", 0} {}

bool TimestampedEvent::operator<(const TimestampedEvent &rhs) const {
    return std::tie(millisSinceEpoch,event_name) < std::tie(rhs.millisSinceEpoch,rhs.event_name);
}

bool TimestampedEvent::operator>(const TimestampedEvent &rhs) const {
    return rhs < *this;
}

bool TimestampedEvent::operator<=(const TimestampedEvent &rhs) const {
    return !(rhs < *this);
}

bool TimestampedEvent::operator>=(const TimestampedEvent &rhs) const {
    return !(*this < rhs);
}
