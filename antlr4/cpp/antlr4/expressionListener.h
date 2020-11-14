
// Generated from antlr4/expression.g by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "expressionParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by expressionParser.
 */
class  expressionListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterZip(expressionParser::ZipContext *ctx) = 0;
  virtual void exitZip(expressionParser::ZipContext *ctx) = 0;

  virtual void enterDiv(expressionParser::DivContext *ctx) = 0;
  virtual void exitDiv(expressionParser::DivContext *ctx) = 0;

  virtual void enterFold(expressionParser::FoldContext *ctx) = 0;
  virtual void exitFold(expressionParser::FoldContext *ctx) = 0;

  virtual void enterMinus(expressionParser::MinusContext *ctx) = 0;
  virtual void exitMinus(expressionParser::MinusContext *ctx) = 0;

  virtual void enterParen(expressionParser::ParenContext *ctx) = 0;
  virtual void exitParen(expressionParser::ParenContext *ctx) = 0;

  virtual void enterTimes(expressionParser::TimesContext *ctx) = 0;
  virtual void exitTimes(expressionParser::TimesContext *ctx) = 0;

  virtual void enterAbs(expressionParser::AbsContext *ctx) = 0;
  virtual void exitAbs(expressionParser::AbsContext *ctx) = 0;

  virtual void enterSqrt(expressionParser::SqrtContext *ctx) = 0;
  virtual void exitSqrt(expressionParser::SqrtContext *ctx) = 0;

  virtual void enterPow(expressionParser::PowContext *ctx) = 0;
  virtual void exitPow(expressionParser::PowContext *ctx) = 0;

  virtual void enterPlus(expressionParser::PlusContext *ctx) = 0;
  virtual void exitPlus(expressionParser::PlusContext *ctx) = 0;


};

