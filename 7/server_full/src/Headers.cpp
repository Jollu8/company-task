//
// Created by Jollu Emil.
//

#pragma once

#include "../include/Headers.h"

#include <string>

namespace http {
using namespace std::string_literals;
std::string const Headers::HOST("Host"s);
std::string const Headers::CONTENT_LENGTH("Content-Length"s);
std::string const Headers::ACCEPT_ENCODING("Accept-Encoding"s);
std::string const Headers::CONNECTION("Connection"s);

std::string Headers::header(const std::string &name) const {
    auto i = m_header.find(name);
    return (i == m_header.end()) ? "" : i->second;
}

void Headers::headerIs(const std::string &name, const std::string &value) {
    m_header.emplace(name, value);
}
}  // namespace http

//


