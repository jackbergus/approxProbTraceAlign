/*
 * xml_utils.cpp
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
#include <utils/xml_utils.h>
#include <fstream>
#include <cassert>

rapidxml::xml_node<> *
init_XML_parse(const std::string &filename, const std::string &root_label, rapidxml::xml_document<> &doc, std::vector<char>& x) {
    std::ifstream file{filename};
    // Read the xml file into a vector
    {
        std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        buffer.push_back('\0');
        x = buffer;
    }
    // Parse the buffer using the xml file parsing library into doc
    doc.parse<0>(x.data());
    rapidxml::xml_node<> * root_node = doc.first_node(root_label.c_str(), 0, false);
    return root_node;
}
