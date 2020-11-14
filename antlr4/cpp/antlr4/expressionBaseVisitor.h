
// Generated from antlr4/expression.g by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "expressionVisitor.h"


/**
 * This class provides an empty implementation of expressionVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  expressionBaseVisitor : public expressionVisitor {
public:

  virtual antlrcpp::Any visitZip(expressionParser::ZipContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDiv(expressionParser::DivContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFold(expressionParser::FoldContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMinus(expressionParser::MinusContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitParen(expressionParser::ParenContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTimes(expressionParser::TimesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAbs(expressionParser::AbsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSqrt(expressionParser::SqrtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPow(expressionParser::PowContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPlus(expressionParser::PlusContext *ctx) override {
    return visitChildren(ctx);
  }


};

