//
// Created by giacomo on 14/11/20.
//

#include "topk/Aussageform/binary_expression_compiler.h"
#include "topk/Aussageform/ExpressionEvaluator.h"
#include <expressionLexer.h>
#include <json.h>

ExpressionEvaluator::ExpressionEvaluator(const std::string &expr) : ExpressionEvaluator() {
    orig_for_move_or_copy = expr;
    sstr.str(expr);
    stream.load(sstr);
    allocateAnew();
    // preload the expression to be interpreted
}

void ExpressionEvaluator::allocateAnew() {
    lexer = new expressionLexer(&stream);
    tokens = new antlr4::CommonTokenStream(lexer);
    tokens->fill();
    parser = new expressionParser(tokens);
    parser->setBuildParseTree(true);
    expr = parser->expression();
}

double ExpressionEvaluator::operator()(const std::vector<double> &lhs, const std::vector<double> &rhs) {
    arguments.clear();
    arguments.emplace_back(lhs);
    arguments.emplace_back(rhs);
    return visit(expr).as<double>();
}

antlrcpp::Any ExpressionEvaluator::visitZip(expressionParser::ZipContext *ctx) {
    binary_expression_compiler expr1{UNESCAPE(ctx->STRING(0)->getText())};
    binary_expression_compiler expr2{UNESCAPE(ctx->STRING(1)->getText())};
    double init = std::stod(ctx->NUMBER()->getText());
    const auto x = arguments[std::stol(ctx->VECTOR(0)->getText().substr(1))];
    const auto y = arguments[std::stol(ctx->VECTOR(1)->getText().substr(1))];
    return std::inner_product(x.begin(), x.end(), y.begin(), init, [&expr1](double x, double y) { return expr1(x,y); }, [&expr2](double x, double y) { return expr2(x,y); });
}

antlrcpp::Any ExpressionEvaluator::visitDiv(expressionParser::DivContext *ctx) {
    return visit(ctx->expression(0)).as<double>()/visit(ctx->expression(1)).as<double>();
}

antlrcpp::Any ExpressionEvaluator::visitFold(expressionParser::FoldContext *ctx) {
    binary_expression_compiler expr{UNESCAPE(ctx->STRING()->getText())};
    double init = std::stod(ctx->NUMBER()->getText());
    std::string txt = ctx->VECTOR()->getText().substr(1);
    size_t i = std::stol(ctx->VECTOR()->getText().substr(1));
    const auto x = arguments[i];
    return std::accumulate(x.begin(), x.end(), init, [&expr](double x, double y) { return expr(x,y); });
}

antlrcpp::Any ExpressionEvaluator::visitMinus(expressionParser::MinusContext *ctx) {
    return visit(ctx->expression()[0]).as<double>()-visit(ctx->expression()[1]).as<double>();
}

antlrcpp::Any ExpressionEvaluator::visitParen(expressionParser::ParenContext *ctx) {
    return visit(ctx->expression());
}

antlrcpp::Any ExpressionEvaluator::visitTimes(expressionParser::TimesContext *ctx) {
    return visit(ctx->expression()[0]).as<double>()*visit(ctx->expression()[1]).as<double>();
}

antlrcpp::Any ExpressionEvaluator::visitAbs(expressionParser::AbsContext *ctx) {
    return std::abs(visit(ctx->expression()).as<double>());
}

antlrcpp::Any ExpressionEvaluator::visitSqrt(expressionParser::SqrtContext *ctx) {
    return std::sqrt(visit(ctx->expression()).as<double>());
}

antlrcpp::Any ExpressionEvaluator::visitPow(expressionParser::PowContext *ctx) {
    return std::pow(visit(ctx->expression()[0]).as<double>(),visit(ctx->expression()[1]).as<double>());
}

antlrcpp::Any ExpressionEvaluator::visitPlus(expressionParser::PlusContext *ctx) {
    return visit(ctx->expression()[0]).as<double>()+visit(ctx->expression()[1]).as<double>();
}

ExpressionEvaluator::~ExpressionEvaluator() {
    delete lexer;
    delete tokens;
    delete parser;
}

ExpressionEvaluator &ExpressionEvaluator::operator=(const ExpressionEvaluator &x) {
    setCopy(x);
}

void ExpressionEvaluator::setCopy(const ExpressionEvaluator &x) {
    arguments = x.arguments;
    sstr.str(x.orig_for_move_or_copy);
    delete lexer;
    lexer = nullptr;
    delete tokens;
    tokens = nullptr;
    delete parser;
    parser = nullptr;
    allocateAnew();
}

ExpressionEvaluator::ExpressionEvaluator(const ExpressionEvaluator &x) : ExpressionEvaluator() {
    setCopy(x);
}

UnterstuetzenStrategie ExpressionEvaluator::getStrategy() const {
    return strategy;
}

void ExpressionEvaluator::setStrategy(UnterstuetzenStrategie strategy) {
    ExpressionEvaluator::strategy = strategy;
}
