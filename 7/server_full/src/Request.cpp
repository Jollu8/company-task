//
// Created by Jollu Emil.
//

#include "../include/Request.h"

namespace http {
std::string const Request::header(const std::string &name) const {
    return m_headers.header(name);
}

void Request::methodsIs(http::Request::Method method) {
    m_method = method;
}

void Request::uriIs(const http::Uri &path) {
    m_uri = path;
}

void Request::dataIs(const std::string &data) {
    m_data = data;
}

void Request::headerIs(const std::string &name, const std::string &value) {
    m_headers.headerIs(name, value);
}
}  // namespace http
