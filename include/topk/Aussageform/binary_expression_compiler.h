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
