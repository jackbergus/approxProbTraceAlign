
// Generated from antlr4/expression.g by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "expressionParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by expressionParser.
 */
class  expressionVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by expressionParser.
   */
    virtual antlrcpp::Any visitZip(expressionParser::ZipContext *context) = 0;

    virtual antlrcpp::Any visitDiv(expressionParser::DivContext *context) = 0;

    virtual antlrcpp::Any visitFold(expressionParser::FoldContext *context) = 0;

    virtual antlrcpp::Any visitMinus(expressionParser::MinusContext *context) = 0;

    virtual antlrcpp::Any visitParen(expressionParser::ParenContext *context) = 0;

    virtual antlrcpp::Any visitTimes(expressionParser::TimesContext *context) = 0;

    virtual antlrcpp::Any visitAbs(expressionParser::AbsContext *context) = 0;

    virtual antlrcpp::Any visitSqrt(expressionParser::SqrtContext *context) = 0;

    virtual antlrcpp::Any visitPow(expressionParser::PowContext *context) = 0;

    virtual antlrcpp::Any visitPlus(expressionParser::PlusContext *context) = 0;


};

