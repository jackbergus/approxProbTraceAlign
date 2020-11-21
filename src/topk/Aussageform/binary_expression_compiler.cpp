/*
 * binary_expression_compiler.cpp
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

#include "topk/Aussageform/binary_expression_compiler.h"

binary_expression_compiler::binary_expression_compiler(const std::string &binary_expression) {
    exprtk::symbol_table<double> symbol_table;
    symbol_table.add_variable("x", x);
    symbol_table.add_variable("y", y);
    expression.register_symbol_table(symbol_table);
    factorCommandParser.compile(binary_expression, expression);
}

double binary_expression_compiler::operator()(double x, double y) {
    this->x = x;
    this->y = y;
    expression.value();
}
