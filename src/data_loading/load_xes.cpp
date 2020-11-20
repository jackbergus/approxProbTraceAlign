//
// Created by giacomo on 13/11/20.
//

#include <utils/xml_utils.h>
#include <data_loading/local_load_data.h>
#include "data_loading/load_xes.h"

std::vector<Transaction<TimestampedEvent>> load_xes(const std::string &filename) {

    rapidxml::xml_document<> doc;
    std::vector<char> buffer;
    rapidxml::xml_node<> *root_node = init_XML_parse(filename, "log", doc, buffer);
    std::string concept_name{"concept:name"}, timeTimestamp{"time:timestamp"};
    std::vector<Transaction<TimestampedEvent>> log;

    SIBLING_ITERATE(trace, root_node, "trace", false) {
        Transaction<TimestampedEvent>& transaction = log.emplace_back();
        long long int previous = 0;

        SIBLING_ITERATE(event, trace, "event", false) {

            std::string eventName = ".";
            long long int timestamp = 0;

            SIBLING_ITERATE(t, event, "string", false) {
                if (GET_ATTRIBUTE(t, "key") == concept_name) {
                    eventName = GET_ATTRIBUTE(t, "value");
                    break;
                }
            }

            SIBLING_ITERATE(t, event, "date", false) {
                if (GET_ATTRIBUTE(t, "key") == timeTimestamp) {
                    timestamp = parse8601(GET_ATTRIBUTE(t, "value"));
                    break;
                }
            }
            if (timestamp == 0) timestamp += (previous+1);
            transaction.emplace_back(eventName, timestamp);
        }
    }

    return log;
}
