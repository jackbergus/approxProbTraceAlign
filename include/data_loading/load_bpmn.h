/*
 * load_bpmn.h
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

#ifndef FUZZYSTRINGMATCHING2_LOAD_BPMN_H
#define FUZZYSTRINGMATCHING2_LOAD_BPMN_H

#include <vector>
#include <string>
#include <GenericGraph.h>


/**
 * Ingests the output of SplitMiner 2
 * @return
 */
std::vector<GenericGraph<size_t>> load_bpmn(const std::string& filename = "file:///home/giacomo/Immagini/split-miner-2.0/sepsis_out.xml", const std::string& epsilon = ".");


#endif //FUZZYSTRINGMATCHING2_LOAD_BPMN_H
