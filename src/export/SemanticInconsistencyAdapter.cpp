/*
 * SemanticInconsistencyAdapter.cpp
 * This file is part of ProbabilisticTraceAlignment
 *
 * Copyright (C) 2021 - Giacomo Bergami
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
// Created by giacomo on 19/03/21.
//

#include "export/SemanticInconsistencyAdapter.h"


SemanticInconsistencyAdapter::SemanticInconsistencyAdapter(const std::string &varepsilon, bool doNotVisitLoopsTwice,
                                                           size_t maxPathLength, const double minimumPathCost) : unfolder{varepsilon, doNotVisitLoopsTwice, maxPathLength, minimumPathCost} {}


void SemanticInconsistencyAdapter::collect_traces_from_model_clause_as_graph(ReadGraph &graph) {
    logs_from_different_clauses.emplace_back(unfolder.collectPaths(graph));
}


#include <distances/strings/Levenstein.h>

double
SemanticInconsistencyAdapter::get_minimum_alignment_cost(size_t clause_id, const std::string &log_trace_single_char) {
    auto& traces = logs_from_different_clauses.at(clause_id);
    if (traces.empty())
        return log_trace_single_char.size();
    else {
        size_t min_cost = std::numeric_limits<size_t>::max();
        for (const auto& model_trace : traces) {
            min_cost = std::min(
                    GeneralizedLevensteinDistance(model_trace.path, log_trace_single_char), min_cost);
        }
        return (double )min_cost;
    }
}

double SemanticInconsistencyAdapter::I_Sigma_l(const std::vector<std::string> &log_single_char_traces) {
    if (log_single_char_traces.empty()) {
        return 0.0;
    } else {
        double min_cost = std::numeric_limits<double>::max();
        for (const std::string& log_trace_single_char : log_single_char_traces) {
            double tot_inc = 0.0;
            bool skip = false;
            for (size_t i = 0, N = logs_from_different_clauses.size(); i<N; i++) {
                tot_inc += get_minimum_alignment_cost(i, log_trace_single_char);
                if (tot_inc > min_cost) {
                    skip = true;
                    break;
                }
            }
            if (skip) continue; // The previous cost was better: continue
            assert(min_cost > tot_inc);
            min_cost = tot_inc;
        }
        return min_cost;
    }
}

double SemanticInconsistencyAdapter::I_Sigma_hit(const std::vector<std::string> &log_single_char_traces) {
    if (log_single_char_traces.empty()) {
        return 0.0;
    } else {
        size_t min_cost = std::numeric_limits<size_t>::max();
        for (const std::string& log_trace_single_char : log_single_char_traces) {
            size_t tot_inc = 0.0;
            bool skip = false;
            for (size_t i = 0, N = logs_from_different_clauses.size(); i<N; i++) {
                if (get_minimum_alignment_cost(i, log_trace_single_char)>0.0) {
                    tot_inc++;
                    if (tot_inc > min_cost) {
                        skip = true;
                        break;
                    }
                }
            }
            if (skip) continue; // The previous cost was better: continue
            assert(min_cost > tot_inc);
            min_cost = tot_inc;
        }
        return (double)min_cost;
    }
}
