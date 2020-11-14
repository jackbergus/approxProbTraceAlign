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
