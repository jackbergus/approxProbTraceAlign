/*
 * load_bpmn.cpp
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

#include <utils/xml_utils.h>
#include "data_loading/load_bpmn.h"

std::vector<GenericGraph<size_t>> load_bpmn(const std::string &filename, const std::string &epsilon) {


    std::vector<char> buffer;
    rapidxml::xml_document<> doc;
    rapidxml::xml_node<> *root_node = init_XML_parse(filename, "definitions", doc, buffer);
    std::vector<GenericGraph<size_t>> petri_nets;
    if (root_node) {
        SIBLING_ITERATE(bpmn, root_node, "process", false) {
            if (bpmn->name() != std::string("process")) continue;
            size_t nodeId = 0;
            const char* spn_id = GET_ATTRIBUTE(bpmn, "id");
            std::string initial_label;
            std::string final_label;
            GenericGraph<size_t>& graph = petri_nets.emplace_back();
            graph.setName(spn_id);
            graph.setWeight(1.0);
            std::unordered_map<std::string, size_t> label_to_id;

            SIBLING_ITERATE(spn_node, bpmn, "task", false) {
                std::string node_id = GET_ATTRIBUTE(spn_node, "id");
                //std::cerr << node_id << std::endl;
                std::string node_label = GET_ATTRIBUTE(spn_node, "name");

                size_t node_id_ = nodeId;
                auto it = label_to_id.insert(std::make_pair(node_id, node_id_));
                if (it.second) nodeId++; else node_id_ = it.first->second;
                graph.add_node(node_id_, node_label, 1.0);

                if (GET_FIRST_CHILD(spn_node, "standardLoopCharacteristics")) {
                    graph.add_edge(node_id_, node_id_);
                }
            }

            SIBLING_ITERATE(spn_node, bpmn, "exclusiveGateway", false) {
                std::string node_id = GET_ATTRIBUTE(spn_node, "id");
                //std::cerr << node_id << std::endl;
                std::string node_label = epsilon;

                size_t node_id_ = nodeId;
                auto it = label_to_id.insert(std::make_pair(node_id, node_id_));
                if (it.second) nodeId++; else node_id_ = it.first->second;
                graph.add_node(node_id_, node_label, 1.0);
            }

            SIBLING_ITERATE(spn_node, bpmn, "sequenceFlow", false) {
                std::string src = GET_ATTRIBUTE(spn_node, "sourceRef");
                std::string dst = GET_ATTRIBUTE(spn_node, "targetRef");
                auto it = label_to_id.find(src);
                if (it == label_to_id.end()) {// the bpmn could also not set a beginner node asn actual node. So, we need to insert it...
                    label_to_id[src] = nodeId;
                    graph.add_node(nodeId, ".", 1.0);
                    nodeId++;
                }
                it = label_to_id.find(dst);
                if (it == label_to_id.end()) {
                    label_to_id[dst] = nodeId;
                    graph.add_node(nodeId, ".", 1.0);
                    nodeId++;
                }
                graph.add_edge(label_to_id.at(src), label_to_id.at(dst));
            }

            {
                rapidxml::xml_node<> * start =  GET_FIRST_CHILD(bpmn, "startEvent");
                assert(start);
                initial_label = GET_ATTRIBUTE(start, "id");
                graph.setStart(label_to_id.at(initial_label));
            }
            {
                rapidxml::xml_node<> * end =  GET_FIRST_CHILD(bpmn, "endEvent");
                assert(end);
                final_label = GET_ATTRIBUTE(end, "id");
                graph.setEnd(label_to_id.at(final_label));
            }
        }
    }
    return petri_nets;
}
