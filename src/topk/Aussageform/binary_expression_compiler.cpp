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
