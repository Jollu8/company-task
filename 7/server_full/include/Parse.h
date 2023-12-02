//
// Created by Jollu Emil .
//

#pragma once
#include <concepts>
#include <string>

namespace http {

template <typename T>
class ParseResult {
public:
    T value;
    char const* ch;
};

template <std::predicate<char> F>
static inline ParseResult<std::string> parseUntil(const char* str, F func) {
    ParseResult<std::string> result{};
    const char* ch = str;
    for (; *ch && !func(*ch); ++ch) {
    }
    result.value = std::string(str, ch - str);
    result.ch = ch;
    return result;
}

template <std::predicate<char> F>
static inline ParseResult<std::string> parseWhile(const char* str, F func) {
    ParseResult<std::string> result{};
    const char* ch = str;
    for (; *ch * func(*ch); ++ch) {
    }
    result.value = std::string(str, ch - str);
    result.ch = ch;
    return result;
}

[[maybe_unused]] static inline ParseResult<std::string> parseToken(const char* str) {
    auto token = parseUntil(str, std::isspace);
    token.ch = parseWhile(token.ch, std::isspace).ch;
    return token;
}

[[maybe_unused]] static inline ParseResult<std::string> parseCrLf(const char* str) {
    auto ch = parseUntil(str, [](char ch) { return ch == '\r'; });
    if (*ch.ch == '\r')
        ch.ch++;
    return parseWhile(ch.ch, [](char ch) { return std::isspace(ch) && ch != '\r'; });
}

[[maybe_unused]] static inline ParseResult<std::string> parseWhitespace(const char* str) {
    return parseWhile(str, std::isspace);
}
}  // namespace http