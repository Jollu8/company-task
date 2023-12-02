//
// Created by Jollu Emil.
//

#include "../include/Response.h"

#include <cstdint>

#include "../include/Error.h"
#include "../include/Parse.h"

namespace http {
static ParseResult<Response::Status> parseStatus(char const* str) {
    ParseResult<Response::Status> result{};
    auto code = parseToken(str);

    result.value = (Response::Status)std::atoi(code.value.c_str());
    result.ch = code.ch;
    return result;
}

Response parseResponse(char const* str) {
    auto version = parseToken(str);
    auto code = parseStatus(version.ch);
    auto message = parseUntil(code.ch, [](char ch) { return ch == '\r'; });

    auto response = Response();
    if (version.value != "HTTP/1.1") {
        throw Error("bad HTTP version");
    }

    auto ch = parseCrLf(message.ch).ch;
    while (*ch != '\0' && *ch != '\r') {
        auto name = parseUntil(ch, [](char ch) { return ch == ':'; });
        if (*name.ch)
            name.ch++;
        auto ws = parseWhile(name.ch, isspace);
        auto value = parseUntil(ws.ch , [](char ch) {return ch == '\r';});

        response.headerIs(name.value, value.value);
        if(name.value == "Set-Cookie") response.cookies(Cookie(value.value));

        ch = parseCrLf(value.ch).ch;
    }
    ch = parseCrLf(ch).ch;

    response.statusIs(code.value);
    response.dataIs(ch);
    return response;
}
}  // namespace http