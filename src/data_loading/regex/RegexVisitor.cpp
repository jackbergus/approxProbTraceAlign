//
// Created by giacomo on 19/10/20.
//

#include <json.h>
#include "data_loading/regex/RegexVisitor.h"
#include "data_loading/regex/Regex.h"

antlrcpp::Any RegexVisitor::visitParen(regexParser::ParenContext *ctx)  {
    return visit(ctx->regex());
}

antlrcpp::Any RegexVisitor::visitStar(regexParser::StarContext *ctx) {
    auto child = visit(ctx->regex()).as<Regex<std::string>*>();
    double value = 1.0;
    if (ctx->NUMBER()) {
        value = std::stod(ctx->NUMBER()->getText());
    }
    return {new Regex<std::string>("", child, Regex_cases::Star, value)};
}

antlrcpp::Any RegexVisitor::visitBinaryAlternative(regexParser::BinaryAlternativeContext *ctx) {
    const auto& vec = ctx->regex();
    assert(vec.size() == 2);
    auto left = visit(vec[0]).as<Regex<std::string>*>();
    auto right = visit(vec[1]).as<Regex<std::string>*>();
    double value = 1.0;
    if (ctx->NUMBER()) {
        value = std::stod(ctx->NUMBER()->getText());
    }
    return {new Regex<std::string>(left, right, value)};
}

antlrcpp::Any RegexVisitor::visitConsequent(regexParser::ConsequentContext *ctx) {
    const auto& vec = ctx->regex();
    assert(vec.size() == 2);
    auto left = visit(vec[0]).as<Regex<std::string>*>();
    auto right = visit(vec[1]).as<Regex<std::string>*>();
    double value = 1.0;
    if (ctx->NUMBER()) {
        value = std::stod(ctx->NUMBER()->getText());
    }
    return {new Regex<std::string>(left, right, value, Regex_cases::Consequent)};
}

antlrcpp::Any RegexVisitor::visitOptional(regexParser::OptionalContext *ctx) {
    auto child = visit(ctx->regex()).as<Regex<std::string>*>();
    double value = 1.0;
    if (ctx->NUMBER()) {
        value = std::stod(ctx->NUMBER()->getText());
    }
    return {new Regex<std::string>("", child, Regex_cases::Optional, value)};
}

antlrcpp::Any RegexVisitor::visitPlus(regexParser::PlusContext *ctx) {
    auto child = visit(ctx->regex()).as<Regex<std::string>*>();
    double value = 1.0;
    if (ctx->NUMBER()) {
        value = std::stod(ctx->NUMBER()->getText());
    }
    return {new Regex<std::string>("", child, Regex_cases::Plus, value)};
}

antlrcpp::Any RegexVisitor::visitString(regexParser::StringContext *ctx) {
    return {new Regex<std::string>(UNESCAPE(ctx->STRING()->getText()))};
}
