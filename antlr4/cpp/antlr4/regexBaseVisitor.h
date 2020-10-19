
// Generated from antlr4/regex.g by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "regexVisitor.h"


/**
 * This class provides an empty implementation of regexVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  regexBaseVisitor : public regexVisitor {
public:

  virtual antlrcpp::Any visitParen(regexParser::ParenContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitString(regexParser::StringContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStar(regexParser::StarContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBinaryAlternative(regexParser::BinaryAlternativeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConsequent(regexParser::ConsequentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOptional(regexParser::OptionalContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPlus(regexParser::PlusContext *ctx) override {
    return visitChildren(ctx);
  }


};

