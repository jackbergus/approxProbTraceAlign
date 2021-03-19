/*
 * SemanticInconsistencyAdapter.h
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

#ifndef FUZZYSTRINGMATCHING2_SEMANTICINCONSISTENCYADAPTER_H
#define FUZZYSTRINGMATCHING2_SEMANTICINCONSISTENCYADAPTER_H


#include <export/MultiplePathsEmbeddingConcrete.h>

struct SemanticInconsistencyAdapter {
    using weighted_log   = std::vector<struct path_info>;
    std::vector<weighted_log> logs_from_different_clauses;

    MultiplePathsEmbeddingConcrete unfolder;

    SemanticInconsistencyAdapter(const std::string& varepsilon = "", bool doNotVisitLoopsTwice = true, size_t maxPathLength = std::numeric_limits<size_t>::max(),
                                 const double minimumPathCost = 2.0 * std::numeric_limits<double>::epsilon());


    void collect_traces_from_model_clause_as_graph(ReadGraph& graph);

    double get_minimum_alignment_cost(size_t clause_id, const std::string& log_trace_single_char);

    double I_Sigma_l(const std::vector<std::string>& log_single_char_traces);
    double I_Sigma_hit(const std::vector<std::string>& log_single_char_traces);
};



#endif //FUZZYSTRINGMATCHING2_SEMANTICINCONSISTENCYADAPTER_H
