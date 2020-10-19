//
// Created by giacomo on 19/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_JSON_H
#define FUZZYSTRINGMATCHING2_JSON_H

#include <cstdio>
#include <nlohmann/json.hpp>

#define UNESCAPE(x)                  nlohmann::json::parse(x).get<std::string>()

#endif //FUZZYSTRINGMATCHING2_JSON_H
