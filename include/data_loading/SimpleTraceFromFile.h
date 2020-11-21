/*
 * SimpleTraceFromFile.h
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
// Created by giacomo on 11/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_SIMPLETRACEFROMFILE_H
#define FUZZYSTRINGMATCHING2_SIMPLETRACEFROMFILE_H

#include <fptree.hpp>
#include <string>
#include "local_load_data.h"

/**
 * Reads one single transaction from a string, where a separator is used to separate each event
 * @param transaction_line  Line to be parsed
 * @param sep               Separation
 * @return                  The resulting transaction
 */
Transaction<TimestampedEvent> transaction_from_string(const std::string& transaction_line, const char sep = ';');

std::vector<Transaction<TimestampedEvent>> read_log(const std::string& file_path, const char sep = ';');

template <typename T>
size_t countFreq(const Transaction<T> &pat, const Transaction<T> &txt) {
    size_t M = pat.size();
    size_t N = txt.size();
    size_t res = 0;

    if (pat.empty()) return txt.size();

    /* A loop to slide pat[] one by one */
    for (int i = 0; i <= N - M; i++)
    {
        /* For current index i, check for
           pattern match */
        int j;
        for (j = 0; j < M; j++)
            if (txt[i+j] != pat[j])
                break;

        // if pat[0...M-1] = txt[i, i+1, ...i+M-1]
        if (j == M)
        {
            res++;
            j = 0;
        }
    }
    return res;
}


size_t countFreq2(const Transaction<std::string> &pat, const Transaction<TimestampedEvent> &txt);

namespace spd_we {

            /**
             * Counts the number of transactions in a log that start with a given activity
             * @param L         Log
             * @param label     Activity
             * @return
             */
    double q_I(const std::vector<Transaction<TimestampedEvent>>& L, const std::string& label, const std::string& varepsilon);


    /**
     * Counts the number of transactions in a log ending with a given activity
     * @param L         Log
     * @param label     Activity
     * @return
     */
    double q_F(const std::vector<Transaction<TimestampedEvent>>& L, const std::string& label, const std::string& varepsilon);

    /**
     * Counts the number of transactions in a log having a given subsequence (s,t)
     * @param L         Log
     * @param s
     * @param t
     * @return
     */
    double q_P(const std::vector<Transaction<TimestampedEvent>>& L, const std::string& s, const std::string& t, const std::string& varepsilon);

    double w_freq(const std::vector<Transaction<TimestampedEvent>>& L, const std::string&t_label, const std::string& varepsilon);

}


#endif //FUZZYSTRINGMATCHING2_SIMPLETRACEFROMFILE_H
