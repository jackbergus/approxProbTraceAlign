//
// Created by giacomo on 14/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_EXPRESSIONEVALUATOR_H
#define FUZZYSTRINGMATCHING2_EXPRESSIONEVALUATOR_H

namespace antlr4 {
    class ANTLRInputStream;
    class CommonTokenStream;
}
class expressionLexer;
class expressionParser;
#include <expressionBaseVisitor.h>
#include <sstream>
#include <topk/topk.h>

class ExpressionEvaluator : public expressionBaseVisitor {
    std::vector<std::vector<double>> arguments;
    const char empty[0] = {};
    antlr4::ANTLRInputStream stream;
    std::istringstream sstr;
    expressionLexer* lexer;
    expressionParser* parser;
    antlr4::CommonTokenStream* tokens;
    expressionParser::ExpressionContext *expr;
    std::string orig_for_move_or_copy;
    UnterstuetzenStrategie strategy;

public:

    UnterstuetzenStrategie getStrategy() const;

    void setStrategy(UnterstuetzenStrategie strategy);

    ExpressionEvaluator() : stream(empty, 0) {}
    ExpressionEvaluator(const std::string& expr);
    ExpressionEvaluator(const ExpressionEvaluator& x);
    ExpressionEvaluator& operator=(const ExpressionEvaluator& x);
    double operator()(const std::vector<double>& lhs, const std::vector<double>& rhs);
    double operator()(const std::vector<double>& lhs);


    ~ExpressionEvaluator();

    antlrcpp::Any visitZip(expressionParser::ZipContext *ctx) override;
    antlrcpp::Any visitDiv(expressionParser::DivContext *ctx) override;
    antlrcpp::Any visitFold(expressionParser::FoldContext *ctx) override;
    antlrcpp::Any visitMinus(expressionParser::MinusContext *ctx) override;
    antlrcpp::Any visitParen(expressionParser::ParenContext *ctx) override;
    antlrcpp::Any visitTimes(expressionParser::TimesContext *ctx) override;
    antlrcpp::Any visitAbs(expressionParser::AbsContext *ctx) override;
    antlrcpp::Any visitSqrt(expressionParser::SqrtContext *ctx) override;
    antlrcpp::Any visitPow(expressionParser::PowContext *ctx) override;
    antlrcpp::Any visitPlus(expressionParser::PlusContext *ctx) override;


private:
    void allocateAnew();
    void setCopy(const ExpressionEvaluator &x);
};


#endif //FUZZYSTRINGMATCHING2_EXPRESSIONEVALUATOR_H
