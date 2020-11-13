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
                std::string node_label = epsilon;

                size_t node_id_ = nodeId;
                auto it = label_to_id.insert(std::make_pair(node_id, node_id_));
                if (it.second) nodeId++; else node_id_ = it.first->second;
                graph.add_node(node_id_, node_label, 1.0);
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

            SIBLING_ITERATE(spn_node, bpmn, "sequenceFlow", false) {
                std::string src = GET_ATTRIBUTE(spn_node, "sourceRef");
                std::string dst = GET_ATTRIBUTE(spn_node, "targetRef");
                graph.add_edge(label_to_id.at(src), label_to_id.at(dst));
            }
        }
    }
    return petri_nets;
}
