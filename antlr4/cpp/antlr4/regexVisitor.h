
// Generated from antlr4/regex.g by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "regexParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by regexParser.
 */
class  regexVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by regexParser.
   */
    virtual antlrcpp::Any visitParen(regexParser::ParenContext *context) = 0;

    virtual antlrcpp::Any visitString(regexParser::StringContext *context) = 0;

    virtual antlrcpp::Any visitStar(regexParser::StarContext *context) = 0;

    virtual antlrcpp::Any visitBinaryAlternative(regexParser::BinaryAlternativeContext *context) = 0;

    virtual antlrcpp::Any visitConsequent(regexParser::ConsequentContext *context) = 0;

    virtual antlrcpp::Any visitOptional(regexParser::OptionalContext *context) = 0;

    virtual antlrcpp::Any visitPlus(regexParser::PlusContext *context) = 0;


};

