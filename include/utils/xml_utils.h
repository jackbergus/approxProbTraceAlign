/*
 * xml_utils.h
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
// Created by giacomo on 11/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_XML_UTILS_H
#define FUZZYSTRINGMATCHING2_XML_UTILS_H

#include <rapidxml/rapidxml.hpp>
#include <string>
#include <vector>

#define SIBLING_ITERATE(curr, root, name, cs)       for (rapidxml::xml_node<> * curr = (root)->first_node(name, 0, cs); curr; curr = curr->next_sibling())
#define SIBLING_ITERATE_CHECK(curr, root, __name, cs)       for (rapidxml::xml_node<> * curr = (root)->first_node(__name, 0, cs); curr; curr = curr->next_sibling()) if (strncmp(curr->name(), __name, curr->name_size())==0)
#define GET_ATTRIBUTE(spn, attr)                    ((spn)->first_attribute(attr)->value())
#define GET_FIRST_CHILD(spn, attr)                  ((spn)->first_node((attr),0,false))
#define STRING_STARTS_WITH(s,with)                  ((s).rfind((with), 0) == 0)

rapidxml::xml_node<> *
init_XML_parse(const std::string &filename, const std::string &root_label, rapidxml::xml_document<> &doc, std::vector<char>& );

#endif //FUZZYSTRINGMATCHING2_XML_UTILS_H
