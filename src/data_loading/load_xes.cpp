//
// Created by giacomo on 13/11/20.
//

#include <utils/xml_utils.h>
#include "data_loading/load_xes.h"

std::vector<Transaction<std::string>> load_xes(const std::string &filename) {

    rapidxml::xml_document<> doc;
    std::vector<char> buffer;
    rapidxml::xml_node<> *root_node = init_XML_parse(filename, "log", doc, buffer);
    std::string concept_name{"concept:name"};
    std::vector<Transaction<std::string>> log;

    SIBLING_ITERATE(trace, root_node, "trace", false) {
        Transaction<std::string>& transaction = log.emplace_back();
        SIBLING_ITERATE(event, trace, "event", false) {
            SIBLING_ITERATE(t, event, "string", false) {
                if (GET_ATTRIBUTE(t, "key") == concept_name) {
                    transaction.emplace_back(GET_ATTRIBUTE(t, "value"));
                }
            }
        }
    }

    return log;
}
