//
// Created by giacomo on 14/11/20.
//

#include "topk/Aussageform/binary_expression_compiler.h"
#include "topk/Aussageform/DistanceExpressionEvaluator.h"
#include <expressionLexer.h>
#include <json.h>

DistanceExpressionEvaluator::DistanceExpressionEvaluator(const std::string &expr) : DistanceExpressionEvaluator() {
    orig_for_move_or_copy = expr;
    sstr.str(expr);
    stream.load(sstr);
    allocateAnew();
    // preload the expression to be interpreted
}

void DistanceExpressionEvaluator::allocateAnew() {
    lexer = new expressionLexer(&stream);
    tokens = new antlr4::CommonTokenStream(lexer);
    tokens->fill();
    parser = new expressionParser(tokens);
    parser->setBuildParseTree(true);
    expr = parser->expression();
}

double DistanceExpressionEvaluator::operator()(const std::vector<double> &lhs, const std::vector<double> &rhs) {
    arguments.clear();
    arguments.emplace_back(lhs);
    arguments.emplace_back(rhs);
    return visit(expr).as<double>();
}

antlrcpp::Any DistanceExpressionEvaluator::visitZip(expressionParser::ZipContext *ctx) {
    binary_expression_compiler expr1{UNESCAPE(ctx->STRING(0)->getText())};
    binary_expression_compiler expr2{UNESCAPE(ctx->STRING(1)->getText())};
    double init = std::stod(ctx->NUMBER()->getText());
    const auto x = arguments[std::stol(ctx->VECTOR(0)->getText().substr(1))];
    const auto y = arguments[std::stol(ctx->VECTOR(1)->getText().substr(1))];
    return std::inner_product(x.begin(), x.end(), y.begin(), init, [&expr1](double x, double y) { return expr1(x,y); }, [&expr2](double x, double y) { return expr2(x,y); });
}

antlrcpp::Any DistanceExpressionEvaluator::visitDiv(expressionParser::DivContext *ctx) {
    return visit(ctx->expression(0)).as<double>()/visit(ctx->expression(1)).as<double>();
}

antlrcpp::Any DistanceExpressionEvaluator::visitFold(expressionParser::FoldContext *ctx) {
    binary_expression_compiler expr{UNESCAPE(ctx->STRING()->getText())};
    double init = std::stod(ctx->NUMBER()->getText());
    std::string txt = ctx->VECTOR()->getText().substr(1);
    size_t i = std::stol(ctx->VECTOR()->getText().substr(1));
    const auto x = arguments[i];
    return std::accumulate(x.begin(), x.end(), init, [&expr](double x, double y) { return expr(x,y); });
}

antlrcpp::Any DistanceExpressionEvaluator::visitMinus(expressionParser::MinusContext *ctx) {
    return visit(ctx->expression()[0]).as<double>()-visit(ctx->expression()[1]).as<double>();
}

antlrcpp::Any DistanceExpressionEvaluator::visitParen(expressionParser::ParenContext *ctx) {
    return visit(ctx->expression());
}

antlrcpp::Any DistanceExpressionEvaluator::visitTimes(expressionParser::TimesContext *ctx) {
    return visit(ctx->expression()[0]).as<double>()*visit(ctx->expression()[1]).as<double>();
}

antlrcpp::Any DistanceExpressionEvaluator::visitAbs(expressionParser::AbsContext *ctx) {
    return std::abs(visit(ctx->expression()).as<double>());
}

antlrcpp::Any DistanceExpressionEvaluator::visitSqrt(expressionParser::SqrtContext *ctx) {
    return std::sqrt(visit(ctx->expression()).as<double>());
}

antlrcpp::Any DistanceExpressionEvaluator::visitPow(expressionParser::PowContext *ctx) {
    return std::pow(visit(ctx->expression()[0]).as<double>(),visit(ctx->expression()[1]).as<double>());
}

antlrcpp::Any DistanceExpressionEvaluator::visitPlus(expressionParser::PlusContext *ctx) {
    return visit(ctx->expression()[0]).as<double>()+visit(ctx->expression()[1]).as<double>();
}

DistanceExpressionEvaluator::~DistanceExpressionEvaluator() {
    delete lexer;
    delete tokens;
    delete parser;
}

DistanceExpressionEvaluator &DistanceExpressionEvaluator::operator=(const DistanceExpressionEvaluator &x) {
    setCopy(x);
    return *this;
}

void DistanceExpressionEvaluator::setCopy(const DistanceExpressionEvaluator &x) {
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

DistanceExpressionEvaluator::DistanceExpressionEvaluator(const DistanceExpressionEvaluator &x) : DistanceExpressionEvaluator() {
    setCopy(x);
}

UnterstuetzenStrategie DistanceExpressionEvaluator::getStrategy() const {
    return strategy;
}

void DistanceExpressionEvaluator::setStrategy(UnterstuetzenStrategie strategy) {
    DistanceExpressionEvaluator::strategy = strategy;
}

double DistanceExpressionEvaluator::operator()(const std::vector<double> &lhs) {
    static std::vector<double> probsim{1.0, 1.0};
    static std::vector<double> transform{0.0, 0.0};
    assert(strategy != EuclideanSpace);
    assert(lhs.size() == 2);
    switch (strategy) {
        case ProbabilitySimilarity:
            return std::abs(operator()(lhs, probsim));

        case TransformedSpace:
            return operator()(lhs, transform);

        case EuclideanSpace:
            return 0.0; // should never reach this part
    }
}
