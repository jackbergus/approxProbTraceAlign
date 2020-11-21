/*
 * RegexVisitor.h
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

#ifndef FUZZYSTRINGMATCHING2_REGEXVISITOR_H
#define FUZZYSTRINGMATCHING2_REGEXVISITOR_H

#include <regexBaseVisitor.h>

class RegexVisitor : public regexBaseVisitor {
public:
    ~RegexVisitor()  {}

    antlrcpp::Any visitParen(regexParser::ParenContext *ctx) override;
    antlrcpp::Any visitString(regexParser::StringContext *ctx) override;
    antlrcpp::Any visitStar(regexParser::StarContext *ctx) override;
    antlrcpp::Any visitBinaryAlternative(regexParser::BinaryAlternativeContext *ctx) override;
    antlrcpp::Any visitConsequent(regexParser::ConsequentContext *ctx) override;
    antlrcpp::Any visitOptional(regexParser::OptionalContext *ctx) override;
    antlrcpp::Any visitPlus(regexParser::PlusContext *ctx) override;
};


#endif //FUZZYSTRINGMATCHING2_REGEXVISITOR_H
