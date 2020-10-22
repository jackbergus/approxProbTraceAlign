//
// Created by giacomo on 19/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_INPUTSTREAM_H
#define FUZZYSTRINGMATCHING2_INPUTSTREAM_H

#include <fstream>
#include <ANTLRInputStream.h>
#include <ReadGraph.h>
#include <regexLexer.h>
#include <regexParser.h>
#include <ThomsonNFA.h>
#include <RegexVisitor.h>

enum input_format {
    REGEX,
    READGRAPH
};

struct ReadGraphConfiguration {
    std::string str;
    bool isFile;
    double hasCost;
    input_format format;

    ReadGraphConfiguration(const std::string &str, bool isFile, double hasCost, input_format format);
    ReadGraphConfiguration() = default;
    ReadGraphConfiguration& operator=(const ReadGraphConfiguration&) = default;
    ReadGraphConfiguration(const ReadGraphConfiguration&) = default;
};

class InputStream {
    std::ifstream file;
    const char empty[0] = {};
    antlr4::ANTLRInputStream stream;
    std::istringstream sstr;

    void openRegexFile(const std::string& filename);
    void openRegexString(const std::string& string);
    Regex<std::string>* openRegex(const std::string& str, bool isFile);

public:
    InputStream() : stream(empty, 0) {}

    ReadGraph readGraph(const std::string& str, bool isFile, double hasCost, input_format format);
    ReadGraph readGraph(const ReadGraphConfiguration &grafo);
    ~InputStream();

};


#endif //FUZZYSTRINGMATCHING2_INPUTSTREAM_H
