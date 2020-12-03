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
#include "petrinet/PetriNet.h"

std::vector<GenericGraph<size_t>> load_pnml(const std::string &filename, const std::string &epsilon) {
    std::regex regex{"a\\d+"};
    std::vector<char> buffer;
    std::string weight_label{"weight"};
    std::string initial_label;

    rapidxml::xml_document<> doc;
    rapidxml::xml_node<> *root_node = init_XML_parse(filename, "pnml", doc, buffer);

    std::vector<GenericGraph<size_t>> petri_nets;

    if (root_node) {
        SIBLING_ITERATE(spn, root_node, "net", false) {
            const char* spn_id = GET_ATTRIBUTE(spn, "id");
            SIBLING_ITERATE(spn_page, spn, "page", false) {
                std::unordered_map<std::string, size_t> label_to_id;
                size_t nodeId = 0;
                const char* spn_page_id = GET_ATTRIBUTE(spn_page, "id");
                fixed_bimap<std::string, size_t> textual_id_to_inferred;
                std::string final_name = std::string(spn_id) +"-" + spn_page_id;

                PetriNet pn;
                GenericGraph<size_t>& graph = petri_nets.emplace_back();
                graph.setWeight(1.0);
                graph.setName(final_name);

                // Iterating over the places
                SIBLING_ITERATE_CHECK(spn_node, spn_page, "place", false) {
                        //if (strncmp(spn_node->name(), "place", spn_node->name_size()) == 0) {
                        std::string node_id = GET_ATTRIBUTE(spn_node, "id");
                        size_t node_id_ = nodeId;
                        auto it = label_to_id.insert(std::make_pair(node_id, node_id_));
                        if (it.second) nodeId++; else node_id_ = it.first->second;
                        textual_id_to_inferred.put(node_id, node_id_);
                        pn.places.emplace(node_id_);
                        std::string node_label;
                        //}
                    }

                // Inferring the initial marking
                std::cerr << "MaxSize " << nodeId << std::endl;
                Marking marking(nodeId, 0);
                SIBLING_ITERATE_CHECK(spn_node, spn_page, "place", false) {
                        //if (strncmp(spn_node->name(), "place", spn_node->name_size()) == 0) {
                        rapidxml::xml_node<> * nodeLabel = GET_FIRST_CHILD(spn_node, "initialMarking");
                        if (nodeLabel) {
                            marking[textual_id_to_inferred.getValue(GET_ATTRIBUTE(spn_node, "id"))] =
                                    std::stol(GET_FIRST_CHILD(nodeLabel, "text")->value());
                        }
                        //}
                    }

                // Iterating over the transitions
                SIBLING_ITERATE_CHECK(spn_node, spn_page, "transition", false) {
                        //if (strncmp(spn_node->name(), "transition", spn_node->name_size()) == 0) {
                        std::string node_id = GET_ATTRIBUTE(spn_node, "id");
                        size_t node_id_ = nodeId;
                        auto it = label_to_id.insert(std::make_pair(node_id, node_id_));
                        if (it.second) nodeId++; else node_id_ = it.first->second;
                        pn.transitions.emplace(node_id_);
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
                            node_label = ".";
                        }
                        pn.transition_labelling[node_id_] = node_label;

                        {
                            rapidxml::xml_node<> * toolSpecific = GET_FIRST_CHILD(spn_node, "toolspecific");
                            if (toolSpecific) {
                                SIBLING_ITERATE(toolAttribute, toolSpecific, "property", false) {
                                    if (GET_ATTRIBUTE(toolAttribute, "key") == weight_label) {
                                        weight = std::stod(toolAttribute->value());
                                        pn.firing_weight[node_id_] = weight;
                                    }
                                }
                            }
                        }
                        //}
                    }


                // Retrieving the edges
                SIBLING_ITERATE_CHECK(spn_node, spn_page, "arc", false) {
                        //if (strncmp(spn_node->name(), "arc", spn_node->name_size()) == 0) {
                        std::string src = GET_ATTRIBUTE(spn_node, "source");
                        std::string dst = GET_ATTRIBUTE(spn_node, "target");
                        size_t src_id = label_to_id.at(src);
                        size_t dst_id =  label_to_id.at(dst);
                        pn.w_set_cost(src_id, dst_id, 1.0);
                        //}
                    }

                const auto final = pn.generateMetaReachabilityGraph(marking);
                // Inserting the nodes
                if (final.isFinalEStateAddedAfterwards) {
                    graph.add_node(final.finalEState, epsilon, 1.0);
                } else {

                    graph.add_node(final.finalEState, pn.transition_labelling.at(final.getTransitionIdAssociatedToMetaNodeId(final.finalEState)), 1.0);
                }
                graph.setEnd(final.finalEState);
                if (final.isInitialEStateAddedAfterwards) {
                    graph.add_node(final.initialEState, epsilon, 1.0);
                } else {

                    graph.add_node(final.initialEState, pn.transition_labelling.at(final.getTransitionIdAssociatedToMetaNodeId(final.initialEState)), 1.0);
                }
                graph.setStart(final.initialEState);
                for (const auto& cp: final.node_id_assoc.getElements()) {
                    size_t transitionId = final.getTransitionIdAssociatedToMetaNodeId(cp.first);
                    graph.add_node(cp.first, pn.transition_labelling.at(transitionId), pn.firing_weight.at(transitionId));
                }
                // Inserting the edges
                for (const auto& v : final.outgoingEdges) {
                    for (const auto& vOut : v.second) {
                        graph.add_edge(v.first, vOut.dst, vOut.probability);
                    }
                }

                /*std::cerr << "Removing Solitary Nodes (if any)... " << std::endl;
                graph.transfer_weight_from_nodes_to_edges();
                std::cerr << "Do Closure... " << std::endl;
                graph.doClosure(epsilon);*/
                /*graph.removeSolitaryNodes();
                std::cerr << "Transferring weight from nodes to edges... " << std::endl;

                std::cerr << "Performing the e-closure... " << std::endl;
                //graph.print("out/03_ReadGraph.txt");
                ///
                std::cerr << "Debug rendering... " << std::endl;
                //graph.print("out/04_closed.txt");
                //graph.render("rendering_test.pdf");*/
            }
        }
    } else {
        // return empty list of PPN graphs
    }

    return petri_nets;
}
