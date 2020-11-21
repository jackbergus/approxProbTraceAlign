/*
 * binary_expression_compiler.h
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
// Created by giacomo on 14/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_BINARY_EXPRESSION_COMPILER_H
#define FUZZYSTRINGMATCHING2_BINARY_EXPRESSION_COMPILER_H

#include <exprtk.hpp>

struct binary_expression_compiler {
    double x;
    double y;

    binary_expression_compiler(const binary_expression_compiler&) = default;
    binary_expression_compiler& operator=(const binary_expression_compiler&) = default;

    binary_expression_compiler(const std::string& binary_expression);
    double operator()(double x, double y);

private:
    exprtk::parser<double> factorCommandParser;
    exprtk::expression<double> expression;
};

#endif //FUZZYSTRINGMATCHING2_BINARY_EXPRESSION_COMPILER_H
