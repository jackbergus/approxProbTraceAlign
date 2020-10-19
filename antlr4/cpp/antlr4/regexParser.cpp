
// Generated from antlr4/regex.g by ANTLR 4.8


#include "regexListener.h"
#include "regexVisitor.h"

#include "regexParser.h"


using namespace antlrcpp;
using namespace antlr4;

regexParser::regexParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

regexParser::~regexParser() {
  delete _interpreter;
}

std::string regexParser::getGrammarFileName() const {
  return "regex.g";
}

const std::vector<std::string>& regexParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& regexParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- RegexContext ------------------------------------------------------------------

regexParser::RegexContext::RegexContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t regexParser::RegexContext::getRuleIndex() const {
  return regexParser::RuleRegex;
}

void regexParser::RegexContext::copyFrom(RegexContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ParenContext ------------------------------------------------------------------

regexParser::RegexContext* regexParser::ParenContext::regex() {
  return getRuleContext<regexParser::RegexContext>(0);
}

regexParser::ParenContext::ParenContext(RegexContext *ctx) { copyFrom(ctx); }

void regexParser::ParenContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<regexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParen(this);
}
void regexParser::ParenContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<regexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParen(this);
}

antlrcpp::Any regexParser::ParenContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<regexVisitor*>(visitor))
    return parserVisitor->visitParen(this);
  else
    return visitor->visitChildren(this);
}
//----------------- StringContext ------------------------------------------------------------------

tree::TerminalNode* regexParser::StringContext::STRING() {
  return getToken(regexParser::STRING, 0);
}

regexParser::StringContext::StringContext(RegexContext *ctx) { copyFrom(ctx); }

void regexParser::StringContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<regexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterString(this);
}
void regexParser::StringContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<regexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitString(this);
}

antlrcpp::Any regexParser::StringContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<regexVisitor*>(visitor))
    return parserVisitor->visitString(this);
  else
    return visitor->visitChildren(this);
}
//----------------- StarContext ------------------------------------------------------------------

regexParser::RegexContext* regexParser::StarContext::regex() {
  return getRuleContext<regexParser::RegexContext>(0);
}

tree::TerminalNode* regexParser::StarContext::NUMBER() {
  return getToken(regexParser::NUMBER, 0);
}

regexParser::StarContext::StarContext(RegexContext *ctx) { copyFrom(ctx); }

void regexParser::StarContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<regexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStar(this);
}
void regexParser::StarContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<regexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStar(this);
}

antlrcpp::Any regexParser::StarContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<regexVisitor*>(visitor))
    return parserVisitor->visitStar(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BinaryAlternativeContext ------------------------------------------------------------------

std::vector<regexParser::RegexContext *> regexParser::BinaryAlternativeContext::regex() {
  return getRuleContexts<regexParser::RegexContext>();
}

regexParser::RegexContext* regexParser::BinaryAlternativeContext::regex(size_t i) {
  return getRuleContext<regexParser::RegexContext>(i);
}

tree::TerminalNode* regexParser::BinaryAlternativeContext::NUMBER() {
  return getToken(regexParser::NUMBER, 0);
}

regexParser::BinaryAlternativeContext::BinaryAlternativeContext(RegexContext *ctx) { copyFrom(ctx); }

void regexParser::BinaryAlternativeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<regexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBinaryAlternative(this);
}
void regexParser::BinaryAlternativeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<regexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBinaryAlternative(this);
}

antlrcpp::Any regexParser::BinaryAlternativeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<regexVisitor*>(visitor))
    return parserVisitor->visitBinaryAlternative(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ConsequentContext ------------------------------------------------------------------

std::vector<regexParser::RegexContext *> regexParser::ConsequentContext::regex() {
  return getRuleContexts<regexParser::RegexContext>();
}

regexParser::RegexContext* regexParser::ConsequentContext::regex(size_t i) {
  return getRuleContext<regexParser::RegexContext>(i);
}

tree::TerminalNode* regexParser::ConsequentContext::NUMBER() {
  return getToken(regexParser::NUMBER, 0);
}

regexParser::ConsequentContext::ConsequentContext(RegexContext *ctx) { copyFrom(ctx); }

void regexParser::ConsequentContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<regexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConsequent(this);
}
void regexParser::ConsequentContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<regexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConsequent(this);
}

antlrcpp::Any regexParser::ConsequentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<regexVisitor*>(visitor))
    return parserVisitor->visitConsequent(this);
  else
    return visitor->visitChildren(this);
}
//----------------- OptionalContext ------------------------------------------------------------------

regexParser::RegexContext* regexParser::OptionalContext::regex() {
  return getRuleContext<regexParser::RegexContext>(0);
}

tree::TerminalNode* regexParser::OptionalContext::NUMBER() {
  return getToken(regexParser::NUMBER, 0);
}

regexParser::OptionalContext::OptionalContext(RegexContext *ctx) { copyFrom(ctx); }

void regexParser::OptionalContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<regexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOptional(this);
}
void regexParser::OptionalContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<regexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOptional(this);
}

antlrcpp::Any regexParser::OptionalContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<regexVisitor*>(visitor))
    return parserVisitor->visitOptional(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PlusContext ------------------------------------------------------------------

regexParser::RegexContext* regexParser::PlusContext::regex() {
  return getRuleContext<regexParser::RegexContext>(0);
}

tree::TerminalNode* regexParser::PlusContext::NUMBER() {
  return getToken(regexParser::NUMBER, 0);
}

regexParser::PlusContext::PlusContext(RegexContext *ctx) { copyFrom(ctx); }

void regexParser::PlusContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<regexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPlus(this);
}
void regexParser::PlusContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<regexListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPlus(this);
}

antlrcpp::Any regexParser::PlusContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<regexVisitor*>(visitor))
    return parserVisitor->visitPlus(this);
  else
    return visitor->visitChildren(this);
}

regexParser::RegexContext* regexParser::regex() {
   return regex(0);
}

regexParser::RegexContext* regexParser::regex(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  regexParser::RegexContext *_localctx = _tracker.createInstance<RegexContext>(_ctx, parentState);
  regexParser::RegexContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 0;
  enterRecursionRule(_localctx, 0, regexParser::RuleRegex, precedence);

    size_t _la = 0;

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
    setState(8);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case regexParser::T__0: {
        _localctx = _tracker.createInstance<ParenContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(3);
        match(regexParser::T__0);
        setState(4);
        regex(0);
        setState(5);
        match(regexParser::T__1);
        break;
      }

      case regexParser::STRING: {
        _localctx = _tracker.createInstance<StringContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(7);
        match(regexParser::STRING);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(38);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(36);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<ConsequentContext>(_tracker.createInstance<RegexContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleRegex);
          setState(10);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(12);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == regexParser::NUMBER) {
            setState(11);
            match(regexParser::NUMBER);
          }
          setState(14);
          regex(3);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<BinaryAlternativeContext>(_tracker.createInstance<RegexContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleRegex);
          setState(15);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(16);
          match(regexParser::T__5);
          setState(18);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == regexParser::NUMBER) {
            setState(17);
            match(regexParser::NUMBER);
          }
          setState(20);
          regex(2);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<PlusContext>(_tracker.createInstance<RegexContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleRegex);
          setState(21);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(23);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == regexParser::NUMBER) {
            setState(22);
            match(regexParser::NUMBER);
          }
          setState(25);
          match(regexParser::T__2);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<StarContext>(_tracker.createInstance<RegexContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleRegex);
          setState(26);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(28);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == regexParser::NUMBER) {
            setState(27);
            match(regexParser::NUMBER);
          }
          setState(30);
          match(regexParser::T__3);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<OptionalContext>(_tracker.createInstance<RegexContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleRegex);
          setState(31);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(33);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == regexParser::NUMBER) {
            setState(32);
            match(regexParser::NUMBER);
          }
          setState(35);
          match(regexParser::T__4);
          break;
        }

        default:
          break;
        } 
      }
      setState(40);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

bool regexParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 0: return regexSempred(dynamic_cast<RegexContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool regexParser::regexSempred(RegexContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 2);
    case 1: return precpred(_ctx, 1);
    case 2: return precpred(_ctx, 6);
    case 3: return precpred(_ctx, 5);
    case 4: return precpred(_ctx, 4);

  default:
    break;
  }
  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> regexParser::_decisionToDFA;
atn::PredictionContextCache regexParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN regexParser::_atn;
std::vector<uint16_t> regexParser::_serializedATN;

std::vector<std::string> regexParser::_ruleNames = {
  "regex"
};

std::vector<std::string> regexParser::_literalNames = {
  "", "'('", "')'", "'+'", "'*'", "'\u003F'", "'|'"
};

std::vector<std::string> regexParser::_symbolicNames = {
  "", "", "", "", "", "", "", "INTNUMBER", "NUMBER", "STRING", "WORD", "NEWLINE", 
  "SPACE"
};

dfa::Vocabulary regexParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> regexParser::_tokenNames;

regexParser::Initializer::Initializer() {
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
    0x3, 0xe, 0x2c, 0x4, 0x2, 0x9, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 
    0x2, 0x3, 0x2, 0x3, 0x2, 0x5, 0x2, 0xb, 0xa, 0x2, 0x3, 0x2, 0x3, 0x2, 
    0x5, 0x2, 0xf, 0xa, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x5, 
    0x2, 0x15, 0xa, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x5, 0x2, 0x1a, 0xa, 
    0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x5, 0x2, 0x1f, 0xa, 0x2, 0x3, 0x2, 
    0x3, 0x2, 0x3, 0x2, 0x5, 0x2, 0x24, 0xa, 0x2, 0x3, 0x2, 0x7, 0x2, 0x27, 
    0xa, 0x2, 0xc, 0x2, 0xe, 0x2, 0x2a, 0xb, 0x2, 0x3, 0x2, 0x2, 0x3, 0x2, 
    0x3, 0x2, 0x2, 0x2, 0x2, 0x35, 0x2, 0xa, 0x3, 0x2, 0x2, 0x2, 0x4, 0x5, 
    0x8, 0x2, 0x1, 0x2, 0x5, 0x6, 0x7, 0x3, 0x2, 0x2, 0x6, 0x7, 0x5, 0x2, 
    0x2, 0x2, 0x7, 0x8, 0x7, 0x4, 0x2, 0x2, 0x8, 0xb, 0x3, 0x2, 0x2, 0x2, 
    0x9, 0xb, 0x7, 0xb, 0x2, 0x2, 0xa, 0x4, 0x3, 0x2, 0x2, 0x2, 0xa, 0x9, 
    0x3, 0x2, 0x2, 0x2, 0xb, 0x28, 0x3, 0x2, 0x2, 0x2, 0xc, 0xe, 0xc, 0x4, 
    0x2, 0x2, 0xd, 0xf, 0x7, 0xa, 0x2, 0x2, 0xe, 0xd, 0x3, 0x2, 0x2, 0x2, 
    0xe, 0xf, 0x3, 0x2, 0x2, 0x2, 0xf, 0x10, 0x3, 0x2, 0x2, 0x2, 0x10, 0x27, 
    0x5, 0x2, 0x2, 0x5, 0x11, 0x12, 0xc, 0x3, 0x2, 0x2, 0x12, 0x14, 0x7, 
    0x8, 0x2, 0x2, 0x13, 0x15, 0x7, 0xa, 0x2, 0x2, 0x14, 0x13, 0x3, 0x2, 
    0x2, 0x2, 0x14, 0x15, 0x3, 0x2, 0x2, 0x2, 0x15, 0x16, 0x3, 0x2, 0x2, 
    0x2, 0x16, 0x27, 0x5, 0x2, 0x2, 0x4, 0x17, 0x19, 0xc, 0x8, 0x2, 0x2, 
    0x18, 0x1a, 0x7, 0xa, 0x2, 0x2, 0x19, 0x18, 0x3, 0x2, 0x2, 0x2, 0x19, 
    0x1a, 0x3, 0x2, 0x2, 0x2, 0x1a, 0x1b, 0x3, 0x2, 0x2, 0x2, 0x1b, 0x27, 
    0x7, 0x5, 0x2, 0x2, 0x1c, 0x1e, 0xc, 0x7, 0x2, 0x2, 0x1d, 0x1f, 0x7, 
    0xa, 0x2, 0x2, 0x1e, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x1e, 0x1f, 0x3, 0x2, 
    0x2, 0x2, 0x1f, 0x20, 0x3, 0x2, 0x2, 0x2, 0x20, 0x27, 0x7, 0x6, 0x2, 
    0x2, 0x21, 0x23, 0xc, 0x6, 0x2, 0x2, 0x22, 0x24, 0x7, 0xa, 0x2, 0x2, 
    0x23, 0x22, 0x3, 0x2, 0x2, 0x2, 0x23, 0x24, 0x3, 0x2, 0x2, 0x2, 0x24, 
    0x25, 0x3, 0x2, 0x2, 0x2, 0x25, 0x27, 0x7, 0x7, 0x2, 0x2, 0x26, 0xc, 
    0x3, 0x2, 0x2, 0x2, 0x26, 0x11, 0x3, 0x2, 0x2, 0x2, 0x26, 0x17, 0x3, 
    0x2, 0x2, 0x2, 0x26, 0x1c, 0x3, 0x2, 0x2, 0x2, 0x26, 0x21, 0x3, 0x2, 
    0x2, 0x2, 0x27, 0x2a, 0x3, 0x2, 0x2, 0x2, 0x28, 0x26, 0x3, 0x2, 0x2, 
    0x2, 0x28, 0x29, 0x3, 0x2, 0x2, 0x2, 0x29, 0x3, 0x3, 0x2, 0x2, 0x2, 
    0x2a, 0x28, 0x3, 0x2, 0x2, 0x2, 0xa, 0xa, 0xe, 0x14, 0x19, 0x1e, 0x23, 
    0x26, 0x28, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

regexParser::Initializer regexParser::_init;
