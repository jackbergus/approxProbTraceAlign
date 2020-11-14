
// Generated from antlr4/expression.g by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "expressionListener.h"


/**
 * This class provides an empty implementation of expressionListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  expressionBaseListener : public expressionListener {
public:

  virtual void enterZip(expressionParser::ZipContext * /*ctx*/) override { }
  virtual void exitZip(expressionParser::ZipContext * /*ctx*/) override { }

  virtual void enterDiv(expressionParser::DivContext * /*ctx*/) override { }
  virtual void exitDiv(expressionParser::DivContext * /*ctx*/) override { }

  virtual void enterFold(expressionParser::FoldContext * /*ctx*/) override { }
  virtual void exitFold(expressionParser::FoldContext * /*ctx*/) override { }

  virtual void enterMinus(expressionParser::MinusContext * /*ctx*/) override { }
  virtual void exitMinus(expressionParser::MinusContext * /*ctx*/) override { }

  virtual void enterParen(expressionParser::ParenContext * /*ctx*/) override { }
  virtual void exitParen(expressionParser::ParenContext * /*ctx*/) override { }

  virtual void enterTimes(expressionParser::TimesContext * /*ctx*/) override { }
  virtual void exitTimes(expressionParser::TimesContext * /*ctx*/) override { }

  virtual void enterAbs(expressionParser::AbsContext * /*ctx*/) override { }
  virtual void exitAbs(expressionParser::AbsContext * /*ctx*/) override { }

  virtual void enterSqrt(expressionParser::SqrtContext * /*ctx*/) override { }
  virtual void exitSqrt(expressionParser::SqrtContext * /*ctx*/) override { }

  virtual void enterPow(expressionParser::PowContext * /*ctx*/) override { }
  virtual void exitPow(expressionParser::PowContext * /*ctx*/) override { }

  virtual void enterPlus(expressionParser::PlusContext * /*ctx*/) override { }
  virtual void exitPlus(expressionParser::PlusContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

