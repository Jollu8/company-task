//
// Created by Jollu Emil.
//

#pragma once
#include <map>
#include <string>
#include <sstream>  // Для std::stringstream

namespace http {
class Headers {
public:
    using Map = std::multimap<std::string, std::string>;

    std::string const header(std::string const &name) const {
        auto it = m_header.find(name);
        if (it != m_header.end()) {
            return it->second;
        } else {
            std::stringstream ss;
            ss << "Header '" << name << "' not found";
            throw std::runtime_error(ss.str());
        }
    }

    auto begin() const {
        return m_header.begin();
    }

    auto end() const {
        return m_header.end();
    }

    void headerIs(std::string const &name, std::string const &value) {
        m_header.insert({name, value});
    }

public:  // Statics
    static inline const std::string HOST = "Host";
    static inline const std::string CONTENT_LENGTH = "Content-Length";
    static inline const std::string ACCEPT_ENCODING = "Accept-Encoding";
    static inline const std::string CONNECTION = "Connection";

private:
    Map m_header;
};
}  // namespace http
