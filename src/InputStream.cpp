//
// Created by giacomo on 19/10/20.
//

#include "InputStream.h"

void InputStream::openRegexFile(const std::string &filename) {
    file.open(filename, std::ifstream::binary);
    stream.load(file);
}

void InputStream::openRegexString(const std::string &string) {
    sstr.str(string);
    stream.load(sstr);
}

InputStream::~InputStream() {
    file.close();
}

Regex<std::string> *InputStream::openRegex(const std::string &str, bool isFile) {
    if (isFile) {
        openRegexFile(str);
    } else {
        openRegexString(str);
    }
    regexLexer lexer{&stream};
    //regexLexer* lexer = new regexLexer(&stream);
    // Generate the tokens.
    antlr4::CommonTokenStream tokens(&lexer);

    file.close();

    tokens.fill();

    // Create the translation that will parse the input.
    regexParser parser(&tokens);
    parser.setBuildParseTree(true);
    RegexVisitor visitor;
    return visitor.visit(parser.regex()).as<Regex<std::string>*>();
}

ReadGraph InputStream::readGraph(const std::string &str, bool isFile, double hasCost, input_format format) {
    switch (format) {
        case REGEX: {
            ThomsonNFA nfa{openRegex(str, isFile)};
            return nfa.generateGraph(hasCost);
        }
        case READGRAPH: {
            ThomsonNFA nfa{str, isFile};
            return nfa.generateGraph(hasCost);
        }
    }
}
