/*
 * load_pnml.cpp
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

#include <regex>
#include <utils/xml_utils.h>
#include "data_loading/load_pnml.h"

std::vector<GenericGraph<size_t>> load_pnml(const std::string &filename, const std::string &epsilon) {
    std::regex regex{"a\\d+"};
    std::vector<char> buffer;
    std::string weight_label{"weight"};
    std::string initial_label;

    rapidxml::xml_document<> doc;
    rapidxml::xml_node<> *root_node = init_XML_parse(filename, "pnml", doc, buffer);

    std::unordered_map<std::string, size_t> label_to_id;
    std::vector<GenericGraph<size_t>> petri_nets;

    if (root_node) {
        SIBLING_ITERATE(spn, root_node, "net", false) {
            const char* spn_id = GET_ATTRIBUTE(spn, "id");
            SIBLING_ITERATE(spn_page, spn, "page", false) {
                size_t nodeId = 0;
                const char* spn_page_id = GET_ATTRIBUTE(spn_page, "id");
                GenericGraph<size_t>& graph = petri_nets.emplace_back();
                graph.setName((std::string() + spn_id) + '-' +  spn_page_id);
                graph.setWeight(1.0);
                // Having one petri net

                // Iterating over the nodes
                SIBLING_ITERATE(spn_node, spn_page, "transition", false) {
                    std::string node_id = GET_ATTRIBUTE(spn_node, "id");
                    size_t node_id_ = nodeId;
                    auto it = label_to_id.insert(std::make_pair(node_id, node_id_));
                    if (it.second) nodeId++; else node_id_ = it.first->second;

                    std::string node_label;
                    double weight = 1.0;

                    {
                        rapidxml::xml_node<> * nodeLabel = GET_FIRST_CHILD(spn_node, "name");
                        if (nodeLabel) {
                            nodeLabel = GET_FIRST_CHILD(nodeLabel, "text");
                            node_label = nodeLabel ? nodeLabel->value() : node_id;
                        } else {
                            node_label = node_id;
                        }
                    }

                    if (STRING_STARTS_WITH(node_label, "tau") ||
                        STRING_STARTS_WITH(node_label, "source ") ||
                        STRING_STARTS_WITH(node_label, "sink ") ||
                        STRING_STARTS_WITH(node_label, "newPlace") ||
                        STRING_STARTS_WITH(node_label, "middle ") ||
                        STRING_STARTS_WITH(node_label, "replacement source ") ||
                        std::regex_match(node_label, regex)) {
                        node_label = epsilon;
                    }

                    {
                        rapidxml::xml_node<> * toolSpecific = GET_FIRST_CHILD(spn_node, "toolspecific");
                        if (toolSpecific) {
                            SIBLING_ITERATE(toolAttribute, toolSpecific, "property", false) {
                                if (GET_ATTRIBUTE(toolAttribute, "key") == weight_label) {
                                    weight = std::stod(toolAttribute->value());
                                }
                            }
                        }
                    }

                    graph.add_node(node_id_, node_label, weight);
                    ///std::cout << node_id << "[l='" << node_label << "'] ~= " << weight << std::endl; // TODO!
                }

                // Retrieving the initial state
                SIBLING_ITERATE(spn_node, spn_page, "place", false) {
                    std::string node_id = GET_ATTRIBUTE(spn_node, "id");
                    rapidxml::xml_node<> * nodeLabel = GET_FIRST_CHILD(spn_node, "initialMarking");
                    if (nodeLabel) {
                        assert(initial_label.empty());
                        initial_label = node_id;
                        graph.setStart(label_to_id.at(initial_label));
                        ///std::cout << "Initial state is = " << initial_label << std::endl; // TODO!
                        break; // Stop the iteration, I've already found it!
                    }
                }

                // Retrieving the transitions
                SIBLING_ITERATE(spn_node, spn_page, "arc", false) {
                    std::string src = GET_ATTRIBUTE(spn_node, "source");
                    std::string dst = GET_ATTRIBUTE(spn_node, "target");
                    graph.add_edge(label_to_id.at(src), label_to_id.at(dst), 1.0);
                    ///std::cout << label_to_id.at(src) << "-->" << label_to_id.at(dst) << std::endl; // TODO!
                }

                graph.removeSolitaryNodes();
                graph.inferTerminalNode();
                petri_nets.emplace_back(graph);
            }
        }
    } else {
        // return empty list of PPN graphs
    }
    return petri_nets;
}
