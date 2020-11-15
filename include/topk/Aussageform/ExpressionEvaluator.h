//
// Created by giacomo on 14/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_EXPRESSIONEVALUATOR_H
#define FUZZYSTRINGMATCHING2_EXPRESSIONEVALUATOR_H

#include <expressionBaseVisitor.h>
#include <expressionLexer.h>
#include "binary_expression_compiler.h"
#include <exprtk.hpp>
#include <json.h>
#include <sstream>>

class ExpressionEvaluator : public expressionBaseVisitor {
    std::vector<std::vector<double>> arguments;
    const char empty[0] = {};
    antlr4::ANTLRInputStream stream;
    std::istringstream sstr;
    expressionLexer* lexer;
    expressionParser* parser;
    antlr4::CommonTokenStream* tokens;
    expressionParser::ExpressionContext *expr;

public:
    ExpressionEvaluator(const std::string& expr) : stream(empty, 0) {
        sstr.str(expr);
        stream.load(sstr);
        lexer = new expressionLexer(&stream);
        tokens = new antlr4::CommonTokenStream(lexer);
        tokens->fill();
        parser = new expressionParser(tokens);
        parser->setBuildParseTree(true);
        this->expr = parser->expression(); // preload the expression to be interpreted
    }

    double operator()(const std::vector<double>& lhs, const std::vector<double>& rhs) {
        arguments.clear();
        arguments.emplace_back(lhs);
        arguments.emplace_back(rhs);
        return visit(expr).as<double>();
    }

    ~ExpressionEvaluator() {
        delete lexer;
        delete tokens;
        delete parser;
    }

    void addArgument(const std::vector<double>& d) { arguments.emplace_back(d); }


    antlrcpp::Any visitZip(expressionParser::ZipContext *ctx) override {
        binary_expression_compiler expr1{UNESCAPE(ctx->STRING(0)->getText())};
        binary_expression_compiler expr2{UNESCAPE(ctx->STRING(1)->getText())};
        double init = std::stod(ctx->NUMBER()->getText());
        const auto x = arguments[std::stol(ctx->VECTOR(0)->getText().substr(1))];
        const auto y = arguments[std::stol(ctx->VECTOR(1)->getText().substr(1))];
        return std::inner_product(x.begin(), x.end(), y.begin(), init, [&expr1](double x, double y) { return expr1(x,y); }, [&expr2](double x, double y) { return expr2(x,y); });
    }

    antlrcpp::Any visitDiv(expressionParser::DivContext *ctx) override {
        return visit(ctx->expression(0)).as<double>()/visit(ctx->expression(1)).as<double>();
    }

    antlrcpp::Any visitFold(expressionParser::FoldContext *ctx) override {
        binary_expression_compiler expr{UNESCAPE(ctx->STRING()->getText())};
        double init = std::stod(ctx->NUMBER()->getText());
        std::string txt = ctx->VECTOR()->getText().substr(1);
        size_t i = std::stol(ctx->VECTOR()->getText().substr(1));
        const auto x = arguments[i];
        return std::accumulate(x.begin(), x.end(), init, [&expr](double x, double y) { return expr(x,y); });
    }

    antlrcpp::Any visitMinus(expressionParser::MinusContext *ctx) override {
        return visit(ctx->expression()[0]).as<double>()-visit(ctx->expression()[1]).as<double>();
    }

    antlrcpp::Any visitParen(expressionParser::ParenContext *ctx) override {
        return visit(ctx->expression());
    }

    antlrcpp::Any visitTimes(expressionParser::TimesContext *ctx) override {
        return visit(ctx->expression()[0]).as<double>()*visit(ctx->expression()[1]).as<double>();
    }

    antlrcpp::Any visitAbs(expressionParser::AbsContext *ctx) override {
        return std::abs(visit(ctx->expression()).as<double>());
    }

    antlrcpp::Any visitSqrt(expressionParser::SqrtContext *ctx) override {
        return std::sqrt(visit(ctx->expression()).as<double>());
    }

    antlrcpp::Any visitPow(expressionParser::PowContext *ctx) override {
        return std::pow(visit(ctx->expression()[0]).as<double>(),visit(ctx->expression()[1]).as<double>());
    }

    antlrcpp::Any visitPlus(expressionParser::PlusContext *ctx) override {
        return visit(ctx->expression()[0]).as<double>()+visit(ctx->expression()[1]).as<double>();
    }

};


#endif //FUZZYSTRINGMATCHING2_EXPRESSIONEVALUATOR_H
