//
// Created by Jollu Emil on 11/27/23.
//



#include "../include/Cookies.h"
#include "../include/Parse.h"

#include <string>
#include <cassert>

namespace http {
ParseResult<std::string> parseName(char const* str) {
    return parseUntil(str, [](char ch) { return std::isspace(ch) || ch == '='; });
}

ParseResult<std::string > ParseValue(char const * str) {
    return parseUntil(str, [](char ch) {return ch == ';' || ch == '=';});
}

ParseResult<std::string > parseSeparator(char const * str) {
    if(*str) {
        assert(*str == ';' || *str == '=');
        return parseWhitespace(str +1);
    }else {
        auto result = ParseResult<std::string >{};
        result.ch = str;
        return result;
    }
}

//Cookie parseCookie

}  // namespace http