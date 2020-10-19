
// Generated from antlr4/regex.g by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "regexListener.h"


/**
 * This class provides an empty implementation of regexListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  regexBaseListener : public regexListener {
public:

  virtual void enterParen(regexParser::ParenContext * /*ctx*/) override { }
  virtual void exitParen(regexParser::ParenContext * /*ctx*/) override { }

  virtual void enterString(regexParser::StringContext * /*ctx*/) override { }
  virtual void exitString(regexParser::StringContext * /*ctx*/) override { }

  virtual void enterStar(regexParser::StarContext * /*ctx*/) override { }
  virtual void exitStar(regexParser::StarContext * /*ctx*/) override { }

  virtual void enterBinaryAlternative(regexParser::BinaryAlternativeContext * /*ctx*/) override { }
  virtual void exitBinaryAlternative(regexParser::BinaryAlternativeContext * /*ctx*/) override { }

  virtual void enterConsequent(regexParser::ConsequentContext * /*ctx*/) override { }
  virtual void exitConsequent(regexParser::ConsequentContext * /*ctx*/) override { }

  virtual void enterOptional(regexParser::OptionalContext * /*ctx*/) override { }
  virtual void exitOptional(regexParser::OptionalContext * /*ctx*/) override { }

  virtual void enterPlus(regexParser::PlusContext * /*ctx*/) override { }
  virtual void exitPlus(regexParser::PlusContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

