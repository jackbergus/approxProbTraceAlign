
// Generated from antlr4/expression.g by ANTLR 4.8


#include "expressionLexer.h"


using namespace antlr4;


expressionLexer::expressionLexer(CharStream *input) : Lexer(input) {
  _interpreter = new atn::LexerATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

expressionLexer::~expressionLexer() {
  delete _interpreter;
}

std::string expressionLexer::getGrammarFileName() const {
  return "expression.g";
}

const std::vector<std::string>& expressionLexer::getRuleNames() const {
  return _ruleNames;
}

const std::vector<std::string>& expressionLexer::getChannelNames() const {
  return _channelNames;
}

const std::vector<std::string>& expressionLexer::getModeNames() const {
  return _modeNames;
}

const std::vector<std::string>& expressionLexer::getTokenNames() const {
  return _tokenNames;
}

dfa::Vocabulary& expressionLexer::getVocabulary() const {
  return _vocabulary;
}

const std::vector<uint16_t> expressionLexer::getSerializedATN() const {
  return _serializedATN;
}

const atn::ATN& expressionLexer::getATN() const {
  return _atn;
}




// Static vars and initialization.
std::vector<dfa::DFA> expressionLexer::_decisionToDFA;
atn::PredictionContextCache expressionLexer::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN expressionLexer::_atn;
std::vector<uint16_t> expressionLexer::_serializedATN;

std::vector<std::string> expressionLexer::_ruleNames = {
  "T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "T__6", "T__7", "T__8", 
  "T__9", "T__10", "INTNUMBER", "NUMBER", "VECTOR", "STRING", "SPACE"
};

std::vector<std::string> expressionLexer::_channelNames = {
  "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
};

std::vector<std::string> expressionLexer::_modeNames = {
  "DEFAULT_MODE"
};

std::vector<std::string> expressionLexer::_literalNames = {
  "", "'('", "')'", "'+'", "'-'", "'\\'", "'*'", "'^'", "'sqrt'", "'abs'", 
  "'zip'", "'fold'"
};

std::vector<std::string> expressionLexer::_symbolicNames = {
  "", "", "", "", "", "", "", "", "", "", "", "", "INTNUMBER", "NUMBER", 
  "VECTOR", "STRING", "SPACE"
};

dfa::Vocabulary expressionLexer::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> expressionLexer::_tokenNames;

expressionLexer::Initializer::Initializer() {
  // This code could be in a static initializer lambda, but VS doesn't allow access to private class members from there.
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
    0x2, 0x12, 0x62, 0x8, 0x1, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 
    0x4, 0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 
    0x7, 0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 
    0xb, 0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 
    0xe, 0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 0x9, 0x11, 
    0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x5, 
    0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x8, 0x3, 0x8, 
    0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0xa, 0x3, 0xa, 
    0x3, 0xa, 0x3, 0xa, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xc, 
    0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xd, 0x6, 0xd, 0x45, 0xa, 
    0xd, 0xd, 0xd, 0xe, 0xd, 0x46, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x5, 0xe, 
    0x4c, 0xa, 0xe, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0x10, 0x3, 0x10, 
    0x3, 0x10, 0x3, 0x10, 0x7, 0x10, 0x55, 0xa, 0x10, 0xc, 0x10, 0xe, 0x10, 
    0x58, 0xb, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x11, 0x6, 0x11, 0x5d, 0xa, 
    0x11, 0xd, 0x11, 0xe, 0x11, 0x5e, 0x3, 0x11, 0x3, 0x11, 0x2, 0x2, 0x12, 
    0x3, 0x3, 0x5, 0x4, 0x7, 0x5, 0x9, 0x6, 0xb, 0x7, 0xd, 0x8, 0xf, 0x9, 
    0x11, 0xa, 0x13, 0xb, 0x15, 0xc, 0x17, 0xd, 0x19, 0xe, 0x1b, 0xf, 0x1d, 
    0x10, 0x1f, 0x11, 0x21, 0x12, 0x3, 0x2, 0x4, 0x4, 0x2, 0x24, 0x24, 0x5e, 
    0x5e, 0x4, 0x2, 0xb, 0xc, 0x22, 0x22, 0x2, 0x66, 0x2, 0x3, 0x3, 0x2, 
    0x2, 0x2, 0x2, 0x5, 0x3, 0x2, 0x2, 0x2, 0x2, 0x7, 0x3, 0x2, 0x2, 0x2, 
    0x2, 0x9, 0x3, 0x2, 0x2, 0x2, 0x2, 0xb, 0x3, 0x2, 0x2, 0x2, 0x2, 0xd, 
    0x3, 0x2, 0x2, 0x2, 0x2, 0xf, 0x3, 0x2, 0x2, 0x2, 0x2, 0x11, 0x3, 0x2, 
    0x2, 0x2, 0x2, 0x13, 0x3, 0x2, 0x2, 0x2, 0x2, 0x15, 0x3, 0x2, 0x2, 0x2, 
    0x2, 0x17, 0x3, 0x2, 0x2, 0x2, 0x2, 0x19, 0x3, 0x2, 0x2, 0x2, 0x2, 0x1b, 
    0x3, 0x2, 0x2, 0x2, 0x2, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x2, 0x1f, 0x3, 0x2, 
    0x2, 0x2, 0x2, 0x21, 0x3, 0x2, 0x2, 0x2, 0x3, 0x23, 0x3, 0x2, 0x2, 0x2, 
    0x5, 0x25, 0x3, 0x2, 0x2, 0x2, 0x7, 0x27, 0x3, 0x2, 0x2, 0x2, 0x9, 0x29, 
    0x3, 0x2, 0x2, 0x2, 0xb, 0x2b, 0x3, 0x2, 0x2, 0x2, 0xd, 0x2d, 0x3, 0x2, 
    0x2, 0x2, 0xf, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x11, 0x31, 0x3, 0x2, 0x2, 
    0x2, 0x13, 0x36, 0x3, 0x2, 0x2, 0x2, 0x15, 0x3a, 0x3, 0x2, 0x2, 0x2, 
    0x17, 0x3e, 0x3, 0x2, 0x2, 0x2, 0x19, 0x44, 0x3, 0x2, 0x2, 0x2, 0x1b, 
    0x48, 0x3, 0x2, 0x2, 0x2, 0x1d, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x1f, 0x50, 
    0x3, 0x2, 0x2, 0x2, 0x21, 0x5c, 0x3, 0x2, 0x2, 0x2, 0x23, 0x24, 0x7, 
    0x2a, 0x2, 0x2, 0x24, 0x4, 0x3, 0x2, 0x2, 0x2, 0x25, 0x26, 0x7, 0x2b, 
    0x2, 0x2, 0x26, 0x6, 0x3, 0x2, 0x2, 0x2, 0x27, 0x28, 0x7, 0x2d, 0x2, 
    0x2, 0x28, 0x8, 0x3, 0x2, 0x2, 0x2, 0x29, 0x2a, 0x7, 0x2f, 0x2, 0x2, 
    0x2a, 0xa, 0x3, 0x2, 0x2, 0x2, 0x2b, 0x2c, 0x7, 0x5e, 0x2, 0x2, 0x2c, 
    0xc, 0x3, 0x2, 0x2, 0x2, 0x2d, 0x2e, 0x7, 0x2c, 0x2, 0x2, 0x2e, 0xe, 
    0x3, 0x2, 0x2, 0x2, 0x2f, 0x30, 0x7, 0x60, 0x2, 0x2, 0x30, 0x10, 0x3, 
    0x2, 0x2, 0x2, 0x31, 0x32, 0x7, 0x75, 0x2, 0x2, 0x32, 0x33, 0x7, 0x73, 
    0x2, 0x2, 0x33, 0x34, 0x7, 0x74, 0x2, 0x2, 0x34, 0x35, 0x7, 0x76, 0x2, 
    0x2, 0x35, 0x12, 0x3, 0x2, 0x2, 0x2, 0x36, 0x37, 0x7, 0x63, 0x2, 0x2, 
    0x37, 0x38, 0x7, 0x64, 0x2, 0x2, 0x38, 0x39, 0x7, 0x75, 0x2, 0x2, 0x39, 
    0x14, 0x3, 0x2, 0x2, 0x2, 0x3a, 0x3b, 0x7, 0x7c, 0x2, 0x2, 0x3b, 0x3c, 
    0x7, 0x6b, 0x2, 0x2, 0x3c, 0x3d, 0x7, 0x72, 0x2, 0x2, 0x3d, 0x16, 0x3, 
    0x2, 0x2, 0x2, 0x3e, 0x3f, 0x7, 0x68, 0x2, 0x2, 0x3f, 0x40, 0x7, 0x71, 
    0x2, 0x2, 0x40, 0x41, 0x7, 0x6e, 0x2, 0x2, 0x41, 0x42, 0x7, 0x66, 0x2, 
    0x2, 0x42, 0x18, 0x3, 0x2, 0x2, 0x2, 0x43, 0x45, 0x4, 0x32, 0x3b, 0x2, 
    0x44, 0x43, 0x3, 0x2, 0x2, 0x2, 0x45, 0x46, 0x3, 0x2, 0x2, 0x2, 0x46, 
    0x44, 0x3, 0x2, 0x2, 0x2, 0x46, 0x47, 0x3, 0x2, 0x2, 0x2, 0x47, 0x1a, 
    0x3, 0x2, 0x2, 0x2, 0x48, 0x4b, 0x5, 0x19, 0xd, 0x2, 0x49, 0x4a, 0x7, 
    0x30, 0x2, 0x2, 0x4a, 0x4c, 0x5, 0x19, 0xd, 0x2, 0x4b, 0x49, 0x3, 0x2, 
    0x2, 0x2, 0x4b, 0x4c, 0x3, 0x2, 0x2, 0x2, 0x4c, 0x1c, 0x3, 0x2, 0x2, 
    0x2, 0x4d, 0x4e, 0x7, 0x78, 0x2, 0x2, 0x4e, 0x4f, 0x5, 0x19, 0xd, 0x2, 
    0x4f, 0x1e, 0x3, 0x2, 0x2, 0x2, 0x50, 0x56, 0x7, 0x24, 0x2, 0x2, 0x51, 
    0x55, 0xa, 0x2, 0x2, 0x2, 0x52, 0x53, 0x7, 0x5e, 0x2, 0x2, 0x53, 0x55, 
    0x9, 0x2, 0x2, 0x2, 0x54, 0x51, 0x3, 0x2, 0x2, 0x2, 0x54, 0x52, 0x3, 
    0x2, 0x2, 0x2, 0x55, 0x58, 0x3, 0x2, 0x2, 0x2, 0x56, 0x54, 0x3, 0x2, 
    0x2, 0x2, 0x56, 0x57, 0x3, 0x2, 0x2, 0x2, 0x57, 0x59, 0x3, 0x2, 0x2, 
    0x2, 0x58, 0x56, 0x3, 0x2, 0x2, 0x2, 0x59, 0x5a, 0x7, 0x24, 0x2, 0x2, 
    0x5a, 0x20, 0x3, 0x2, 0x2, 0x2, 0x5b, 0x5d, 0x9, 0x3, 0x2, 0x2, 0x5c, 
    0x5b, 0x3, 0x2, 0x2, 0x2, 0x5d, 0x5e, 0x3, 0x2, 0x2, 0x2, 0x5e, 0x5c, 
    0x3, 0x2, 0x2, 0x2, 0x5e, 0x5f, 0x3, 0x2, 0x2, 0x2, 0x5f, 0x60, 0x3, 
    0x2, 0x2, 0x2, 0x60, 0x61, 0x8, 0x11, 0x2, 0x2, 0x61, 0x22, 0x3, 0x2, 
    0x2, 0x2, 0x8, 0x2, 0x46, 0x4b, 0x54, 0x56, 0x5e, 0x3, 0x8, 0x2, 0x2, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

expressionLexer::Initializer expressionLexer::_init;
