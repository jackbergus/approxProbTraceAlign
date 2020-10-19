
// Generated from antlr4/regex.g by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "regexParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by regexParser.
 */
class  regexListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterParen(regexParser::ParenContext *ctx) = 0;
  virtual void exitParen(regexParser::ParenContext *ctx) = 0;

  virtual void enterString(regexParser::StringContext *ctx) = 0;
  virtual void exitString(regexParser::StringContext *ctx) = 0;

  virtual void enterStar(regexParser::StarContext *ctx) = 0;
  virtual void exitStar(regexParser::StarContext *ctx) = 0;

  virtual void enterBinaryAlternative(regexParser::BinaryAlternativeContext *ctx) = 0;
  virtual void exitBinaryAlternative(regexParser::BinaryAlternativeContext *ctx) = 0;

  virtual void enterConsequent(regexParser::ConsequentContext *ctx) = 0;
  virtual void exitConsequent(regexParser::ConsequentContext *ctx) = 0;

  virtual void enterOptional(regexParser::OptionalContext *ctx) = 0;
  virtual void exitOptional(regexParser::OptionalContext *ctx) = 0;

  virtual void enterPlus(regexParser::PlusContext *ctx) = 0;
  virtual void exitPlus(regexParser::PlusContext *ctx) = 0;


};

