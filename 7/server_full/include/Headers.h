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

    std::string header(std::string const &name) const;

    auto begin() const {
        return m_header.begin();
    }

    auto end() const {
        return m_header.end();
    }

    void headerIs(std::string const &name, std::string const &value);

public:  // Statics
    static  const std::string HOST;
    static  const std::string CONTENT_LENGTH ;
    static  const std::string ACCEPT_ENCODING ;
    static  const std::string CONNECTION ;

private:
    Map m_header;
};
}  // namespace http
