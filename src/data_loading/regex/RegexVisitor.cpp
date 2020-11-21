/*
 * RegexVisitor.cpp
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
// Created by giacomo on 19/10/20.
//

#include <json.h>
#include "data_loading/regex/RegexVisitor.h"
#include "data_loading/regex/Regex.h"

antlrcpp::Any RegexVisitor::visitParen(regexParser::ParenContext *ctx)  {
    return visit(ctx->regex());
}

antlrcpp::Any RegexVisitor::visitStar(regexParser::StarContext *ctx) {
    auto child = visit(ctx->regex()).as<Regex<std::string>*>();
    double value = 1.0;
    if (ctx->NUMBER()) {
        value = std::stod(ctx->NUMBER()->getText());
    }
    return {new Regex<std::string>("", child, Regex_cases::Star, value)};
}

antlrcpp::Any RegexVisitor::visitBinaryAlternative(regexParser::BinaryAlternativeContext *ctx) {
    const auto& vec = ctx->regex();
    assert(vec.size() == 2);
    auto left = visit(vec[0]).as<Regex<std::string>*>();
    auto right = visit(vec[1]).as<Regex<std::string>*>();
    double value = 1.0;
    if (ctx->NUMBER()) {
        value = std::stod(ctx->NUMBER()->getText());
    }
    return {new Regex<std::string>(left, right, value)};
}

antlrcpp::Any RegexVisitor::visitConsequent(regexParser::ConsequentContext *ctx) {
    const auto& vec = ctx->regex();
    assert(vec.size() == 2);
    auto left = visit(vec[0]).as<Regex<std::string>*>();
    auto right = visit(vec[1]).as<Regex<std::string>*>();
    double value = 1.0;
    if (ctx->NUMBER()) {
        value = std::stod(ctx->NUMBER()->getText());
    }
    return {new Regex<std::string>(left, right, value, Regex_cases::Consequent)};
}

antlrcpp::Any RegexVisitor::visitOptional(regexParser::OptionalContext *ctx) {
    auto child = visit(ctx->regex()).as<Regex<std::string>*>();
    double value = 1.0;
    if (ctx->NUMBER()) {
        value = std::stod(ctx->NUMBER()->getText());
    }
    return {new Regex<std::string>("", child, Regex_cases::Optional, value)};
}

antlrcpp::Any RegexVisitor::visitPlus(regexParser::PlusContext *ctx) {
    auto child = visit(ctx->regex()).as<Regex<std::string>*>();
    double value = 1.0;
    if (ctx->NUMBER()) {
        value = std::stod(ctx->NUMBER()->getText());
    }
    return {new Regex<std::string>("", child, Regex_cases::Plus, value)};
}

antlrcpp::Any RegexVisitor::visitString(regexParser::StringContext *ctx) {
    return {new Regex<std::string>(UNESCAPE(ctx->STRING()->getText()))};
}
