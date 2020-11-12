//
// Created by giacomo on 11/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_XML_UTILS_H
#define FUZZYSTRINGMATCHING2_XML_UTILS_H

#include <rapidxml/rapidxml.hpp>
#include <string>
#include <vector>

#define SIBLING_ITERATE(curr, root, name, cs)       for (rapidxml::xml_node<> * curr = (root)->first_node(name, 0, cs); curr; curr = curr->next_sibling())
#define GET_ATTRIBUTE(spn, attr)                    ((spn)->first_attribute(attr)->value())
#define GET_FIRST_CHILD(spn, attr)                  ((spn)->first_node((attr),0,false))
#define STRING_STARTS_WITH(s,with)                  ((s).rfind((with), 0) == 0)

rapidxml::xml_node<> *
init_XML_parse(const std::string &filename, const std::string &root_label, rapidxml::xml_document<> &doc, std::vector<char>& );

#endif //FUZZYSTRINGMATCHING2_XML_UTILS_H
