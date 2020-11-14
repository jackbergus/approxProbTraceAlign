
// Generated from antlr4/expression.g by ANTLR 4.8


#include "expressionListener.h"
#include "expressionVisitor.h"

#include "expressionParser.h"


using namespace antlrcpp;
using namespace antlr4;

expressionParser::expressionParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

expressionParser::~expressionParser() {
  delete _interpreter;
}

std::string expressionParser::getGrammarFileName() const {
  return "expression.g";
}

const std::vector<std::string>& expressionParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& expressionParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- ExpressionContext ------------------------------------------------------------------

expressionParser::ExpressionContext::ExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t expressionParser::ExpressionContext::getRuleIndex() const {
  return expressionParser::RuleExpression;
}

void expressionParser::ExpressionContext::copyFrom(ExpressionContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ZipContext ------------------------------------------------------------------

std::vector<tree::TerminalNode *> expressionParser::ZipContext::STRING() {
  return getTokens(expressionParser::STRING);
}

tree::TerminalNode* expressionParser::ZipContext::STRING(size_t i) {
  return getToken(expressionParser::STRING, i);
}

tree::TerminalNode* expressionParser::ZipContext::NUMBER() {
  return getToken(expressionParser::NUMBER, 0);
}

std::vector<tree::TerminalNode *> expressionParser::ZipContext::VECTOR() {
  return getTokens(expressionParser::VECTOR);
}

tree::TerminalNode* expressionParser::ZipContext::VECTOR(size_t i) {
  return getToken(expressionParser::VECTOR, i);
}

expressionParser::ZipContext::ZipContext(ExpressionContext *ctx) { copyFrom(ctx); }

void expressionParser::ZipContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<expressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterZip(this);
}
void expressionParser::ZipContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<expressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitZip(this);
}

antlrcpp::Any expressionParser::ZipContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<expressionVisitor*>(visitor))
    return parserVisitor->visitZip(this);
  else
    return visitor->visitChildren(this);
}
//----------------- DivContext ------------------------------------------------------------------

std::vector<expressionParser::ExpressionContext *> expressionParser::DivContext::expression() {
  return getRuleContexts<expressionParser::ExpressionContext>();
}

expressionParser::ExpressionContext* expressionParser::DivContext::expression(size_t i) {
  return getRuleContext<expressionParser::ExpressionContext>(i);
}

expressionParser::DivContext::DivContext(ExpressionContext *ctx) { copyFrom(ctx); }

void expressionParser::DivContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<expressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDiv(this);
}
void expressionParser::DivContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<expressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDiv(this);
}

antlrcpp::Any expressionParser::DivContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<expressionVisitor*>(visitor))
    return parserVisitor->visitDiv(this);
  else
    return visitor->visitChildren(this);
}
//----------------- FoldContext ------------------------------------------------------------------

tree::TerminalNode* expressionParser::FoldContext::STRING() {
  return getToken(expressionParser::STRING, 0);
}

tree::TerminalNode* expressionParser::FoldContext::NUMBER() {
  return getToken(expressionParser::NUMBER, 0);
}

tree::TerminalNode* expressionParser::FoldContext::VECTOR() {
  return getToken(expressionParser::VECTOR, 0);
}

expressionParser::FoldContext::FoldContext(ExpressionContext *ctx) { copyFrom(ctx); }

void expressionParser::FoldContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<expressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFold(this);
}
void expressionParser::FoldContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<expressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFold(this);
}

antlrcpp::Any expressionParser::FoldContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<expressionVisitor*>(visitor))
    return parserVisitor->visitFold(this);
  else
    return visitor->visitChildren(this);
}
//----------------- MinusContext ------------------------------------------------------------------

std::vector<expressionParser::ExpressionContext *> expressionParser::MinusContext::expression() {
  return getRuleContexts<expressionParser::ExpressionContext>();
}

expressionParser::ExpressionContext* expressionParser::MinusContext::expression(size_t i) {
  return getRuleContext<expressionParser::ExpressionContext>(i);
}

expressionParser::MinusContext::MinusContext(ExpressionContext *ctx) { copyFrom(ctx); }

void expressionParser::MinusContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<expressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMinus(this);
}
void expressionParser::MinusContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<expressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMinus(this);
}

antlrcpp::Any expressionParser::MinusContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<expressionVisitor*>(visitor))
    return parserVisitor->visitMinus(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ParenContext ------------------------------------------------------------------

expressionParser::ExpressionContext* expressionParser::ParenContext::expression() {
  return getRuleContext<expressionParser::ExpressionContext>(0);
}

expressionParser::ParenContext::ParenContext(ExpressionContext *ctx) { copyFrom(ctx); }

void expressionParser::ParenContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<expressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParen(this);
}
void expressionParser::ParenContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<expressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParen(this);
}

antlrcpp::Any expressionParser::ParenContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<expressionVisitor*>(visitor))
    return parserVisitor->visitParen(this);
  else
    return visitor->visitChildren(this);
}
//----------------- TimesContext ------------------------------------------------------------------

std::vector<expressionParser::ExpressionContext *> expressionParser::TimesContext::expression() {
  return getRuleContexts<expressionParser::ExpressionContext>();
}

expressionParser::ExpressionContext* expressionParser::TimesContext::expression(size_t i) {
  return getRuleContext<expressionParser::ExpressionContext>(i);
}

expressionParser::TimesContext::TimesContext(ExpressionContext *ctx) { copyFrom(ctx); }

void expressionParser::TimesContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<expressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTimes(this);
}
void expressionParser::TimesContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<expressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTimes(this);
}

antlrcpp::Any expressionParser::TimesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<expressionVisitor*>(visitor))
    return parserVisitor->visitTimes(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AbsContext ------------------------------------------------------------------

expressionParser::ExpressionContext* expressionParser::AbsContext::expression() {
  return getRuleContext<expressionParser::ExpressionContext>(0);
}

expressionParser::AbsContext::AbsContext(ExpressionContext *ctx) { copyFrom(ctx); }

void expressionParser::AbsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<expressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAbs(this);
}
void expressionParser::AbsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<expressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAbs(this);
}

antlrcpp::Any expressionParser::AbsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<expressionVisitor*>(visitor))
    return parserVisitor->visitAbs(this);
  else
    return visitor->visitChildren(this);
}
//----------------- SqrtContext ------------------------------------------------------------------

expressionParser::ExpressionContext* expressionParser::SqrtContext::expression() {
  return getRuleContext<expressionParser::ExpressionContext>(0);
}

expressionParser::SqrtContext::SqrtContext(ExpressionContext *ctx) { copyFrom(ctx); }

void expressionParser::SqrtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<expressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSqrt(this);
}
void expressionParser::SqrtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<expressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSqrt(this);
}

antlrcpp::Any expressionParser::SqrtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<expressionVisitor*>(visitor))
    return parserVisitor->visitSqrt(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PowContext ------------------------------------------------------------------

std::vector<expressionParser::ExpressionContext *> expressionParser::PowContext::expression() {
  return getRuleContexts<expressionParser::ExpressionContext>();
}

expressionParser::ExpressionContext* expressionParser::PowContext::expression(size_t i) {
  return getRuleContext<expressionParser::ExpressionContext>(i);
}

expressionParser::PowContext::PowContext(ExpressionContext *ctx) { copyFrom(ctx); }

void expressionParser::PowContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<expressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPow(this);
}
void expressionParser::PowContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<expressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPow(this);
}

antlrcpp::Any expressionParser::PowContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<expressionVisitor*>(visitor))
    return parserVisitor->visitPow(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PlusContext ------------------------------------------------------------------

std::vector<expressionParser::ExpressionContext *> expressionParser::PlusContext::expression() {
  return getRuleContexts<expressionParser::ExpressionContext>();
}

expressionParser::ExpressionContext* expressionParser::PlusContext::expression(size_t i) {
  return getRuleContext<expressionParser::ExpressionContext>(i);
}

expressionParser::PlusContext::PlusContext(ExpressionContext *ctx) { copyFrom(ctx); }

void expressionParser::PlusContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<expressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPlus(this);
}
void expressionParser::PlusContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<expressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPlus(this);
}

antlrcpp::Any expressionParser::PlusContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<expressionVisitor*>(visitor))
    return parserVisitor->visitPlus(this);
  else
    return visitor->visitChildren(this);
}

expressionParser::ExpressionContext* expressionParser::expression() {
   return expression(0);
}

expressionParser::ExpressionContext* expressionParser::expression(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  expressionParser::ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, parentState);
  expressionParser::ExpressionContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 0;
  enterRecursionRule(_localctx, 0, expressionParser::RuleExpression, precedence);

    

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(21);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case expressionParser::T__0: {
        _localctx = _tracker.createInstance<ParenContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(3);
        match(expressionParser::T__0);
        setState(4);
        expression(0);
        setState(5);
        match(expressionParser::T__1);
        break;
      }

      case expressionParser::T__7: {
        _localctx = _tracker.createInstance<SqrtContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(7);
        match(expressionParser::T__7);
        setState(8);
        expression(4);
        break;
      }

      case expressionParser::T__8: {
        _localctx = _tracker.createInstance<AbsContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(9);
        match(expressionParser::T__8);
        setState(10);
        expression(3);
        break;
      }

      case expressionParser::T__9: {
        _localctx = _tracker.createInstance<ZipContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(11);
        match(expressionParser::T__9);
        setState(12);
        match(expressionParser::STRING);
        setState(13);
        match(expressionParser::STRING);
        setState(14);
        match(expressionParser::NUMBER);
        setState(15);
        match(expressionParser::VECTOR);
        setState(16);
        match(expressionParser::VECTOR);
        break;
      }

      case expressionParser::T__10: {
        _localctx = _tracker.createInstance<FoldContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(17);
        match(expressionParser::T__10);
        setState(18);
        match(expressionParser::STRING);
        setState(19);
        match(expressionParser::NUMBER);
        setState(20);
        match(expressionParser::VECTOR);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(40);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(38);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<PlusContext>(_tracker.createInstance<ExpressionContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpression);
          setState(23);

          if (!(precpred(_ctx, 9))) throw FailedPredicateException(this, "precpred(_ctx, 9)");
          setState(24);
          match(expressionParser::T__2);
          setState(25);
          expression(9);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<MinusContext>(_tracker.createInstance<ExpressionContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpression);
          setState(26);

          if (!(precpred(_ctx, 8))) throw FailedPredicateException(this, "precpred(_ctx, 8)");
          setState(27);
          match(expressionParser::T__3);
          setState(28);
          expression(8);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<DivContext>(_tracker.createInstance<ExpressionContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpression);
          setState(29);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
          setState(30);
          match(expressionParser::T__4);
          setState(31);
          expression(7);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<TimesContext>(_tracker.createInstance<ExpressionContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpression);
          setState(32);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(33);
          match(expressionParser::T__5);
          setState(34);
          expression(6);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<PowContext>(_tracker.createInstance<ExpressionContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpression);
          setState(35);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(36);
          match(expressionParser::T__6);
          setState(37);
          expression(5);
          break;
        }

        default:
          break;
        } 
      }
      setState(42);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

bool expressionParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 0: return expressionSempred(dynamic_cast<ExpressionContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool expressionParser::expressionSempred(ExpressionContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 9);
    case 1: return precpred(_ctx, 8);
    case 2: return precpred(_ctx, 7);
    case 3: return precpred(_ctx, 6);
    case 4: return precpred(_ctx, 5);

  default:
    break;
  }
  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> expressionParser::_decisionToDFA;
atn::PredictionContextCache expressionParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN expressionParser::_atn;
std::vector<uint16_t> expressionParser::_serializedATN;

std::vector<std::string> expressionParser::_ruleNames = {
  "expression"
};

std::vector<std::string> expressionParser::_literalNames = {
  "", "'('", "')'", "'+'", "'-'", "'\\'", "'*'", "'^'", "'sqrt'", "'abs'", 
  "'zip'", "'fold'"
};

std::vector<std::string> expressionParser::_symbolicNames = {
  "", "", "", "", "", "", "", "", "", "", "", "", "INTNUMBER", "NUMBER", 
  "VECTOR", "STRING", "SPACE"
};

dfa::Vocabulary expressionParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> expressionParser::_tokenNames;

expressionParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0x12, 0x2e, 0x4, 0x2, 0x9, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 
    0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 
    0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 
    0x2, 0x3, 0x2, 0x5, 0x2, 0x18, 0xa, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 
    0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 
    0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x7, 0x2, 0x29, 0xa, 
    0x2, 0xc, 0x2, 0xe, 0x2, 0x2c, 0xb, 0x2, 0x3, 0x2, 0x2, 0x3, 0x2, 0x3, 
    0x2, 0x2, 0x2, 0x2, 0x35, 0x2, 0x17, 0x3, 0x2, 0x2, 0x2, 0x4, 0x5, 0x8, 
    0x2, 0x1, 0x2, 0x5, 0x6, 0x7, 0x3, 0x2, 0x2, 0x6, 0x7, 0x5, 0x2, 0x2, 
    0x2, 0x7, 0x8, 0x7, 0x4, 0x2, 0x2, 0x8, 0x18, 0x3, 0x2, 0x2, 0x2, 0x9, 
    0xa, 0x7, 0xa, 0x2, 0x2, 0xa, 0x18, 0x5, 0x2, 0x2, 0x6, 0xb, 0xc, 0x7, 
    0xb, 0x2, 0x2, 0xc, 0x18, 0x5, 0x2, 0x2, 0x5, 0xd, 0xe, 0x7, 0xc, 0x2, 
    0x2, 0xe, 0xf, 0x7, 0x11, 0x2, 0x2, 0xf, 0x10, 0x7, 0x11, 0x2, 0x2, 
    0x10, 0x11, 0x7, 0xf, 0x2, 0x2, 0x11, 0x12, 0x7, 0x10, 0x2, 0x2, 0x12, 
    0x18, 0x7, 0x10, 0x2, 0x2, 0x13, 0x14, 0x7, 0xd, 0x2, 0x2, 0x14, 0x15, 
    0x7, 0x11, 0x2, 0x2, 0x15, 0x16, 0x7, 0xf, 0x2, 0x2, 0x16, 0x18, 0x7, 
    0x10, 0x2, 0x2, 0x17, 0x4, 0x3, 0x2, 0x2, 0x2, 0x17, 0x9, 0x3, 0x2, 
    0x2, 0x2, 0x17, 0xb, 0x3, 0x2, 0x2, 0x2, 0x17, 0xd, 0x3, 0x2, 0x2, 0x2, 
    0x17, 0x13, 0x3, 0x2, 0x2, 0x2, 0x18, 0x2a, 0x3, 0x2, 0x2, 0x2, 0x19, 
    0x1a, 0xc, 0xb, 0x2, 0x2, 0x1a, 0x1b, 0x7, 0x5, 0x2, 0x2, 0x1b, 0x29, 
    0x5, 0x2, 0x2, 0xb, 0x1c, 0x1d, 0xc, 0xa, 0x2, 0x2, 0x1d, 0x1e, 0x7, 
    0x6, 0x2, 0x2, 0x1e, 0x29, 0x5, 0x2, 0x2, 0xa, 0x1f, 0x20, 0xc, 0x9, 
    0x2, 0x2, 0x20, 0x21, 0x7, 0x7, 0x2, 0x2, 0x21, 0x29, 0x5, 0x2, 0x2, 
    0x9, 0x22, 0x23, 0xc, 0x8, 0x2, 0x2, 0x23, 0x24, 0x7, 0x8, 0x2, 0x2, 
    0x24, 0x29, 0x5, 0x2, 0x2, 0x8, 0x25, 0x26, 0xc, 0x7, 0x2, 0x2, 0x26, 
    0x27, 0x7, 0x9, 0x2, 0x2, 0x27, 0x29, 0x5, 0x2, 0x2, 0x7, 0x28, 0x19, 
    0x3, 0x2, 0x2, 0x2, 0x28, 0x1c, 0x3, 0x2, 0x2, 0x2, 0x28, 0x1f, 0x3, 
    0x2, 0x2, 0x2, 0x28, 0x22, 0x3, 0x2, 0x2, 0x2, 0x28, 0x25, 0x3, 0x2, 
    0x2, 0x2, 0x29, 0x2c, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x28, 0x3, 0x2, 0x2, 
    0x2, 0x2a, 0x2b, 0x3, 0x2, 0x2, 0x2, 0x2b, 0x3, 0x3, 0x2, 0x2, 0x2, 
    0x2c, 0x2a, 0x3, 0x2, 0x2, 0x2, 0x5, 0x17, 0x28, 0x2a, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

expressionParser::Initializer expressionParser::_init;
