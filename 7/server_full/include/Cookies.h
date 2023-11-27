//
// Created by Jollu Emil .
//
#pragma once
#include <string>
#include <map>

namespace http {

class Cookie {
public:  // Constructor
    explicit Cookie(std::string const& text);
    Cookie() : m_httpOnly(false), m_secure(false){};

public:  // Getters
    [[nodiscard]] std::string const& name() const noexcept {
        return m_name;
    }
    [[nodiscard]] std::string const& value() const noexcept {
        return m_value;
    }
    [[nodiscard]] std::string const& path() const noexcept {
        return m_path;
    }

    [[nodiscard]] bool httpOnly() const noexcept {
        return m_httpOnly;
    }
    [[nodiscard]] bool secure() const noexcept {
        return m_secure;
    }

public:  // Setters
    void nameIs(std::string const& name) {
        m_name = name;
    }
    void valueIs(std::string const& value) {
        m_value = value;
    }
    void pathIs(std::string const& path) {
        m_path = path;
    }

    void httpOnlyIs(bool httpOnly) {
        m_httpOnly = httpOnly;
    }
    void secureIs(bool secure) {
        m_secure = secure;
    }

private:  // Fields
    std::string m_name;
    std::string m_value;
    std::string m_path;
    bool m_httpOnly;
    bool m_secure;
};




class Cookies {
public:
    using Map = std::map<std::string, Cookie>;

    [[nodiscard]] Cookie const &cookie(std::string const& name) const {
        auto it = m_cookie.find(name);
        if (it != m_cookie.end()) {
            return it->second;
        } else {
            throw std::runtime_error("Cookie not found");
        }
    }

    [[nodiscard]] auto begin() const noexcept {
        return m_cookie.begin();
    }

    [[nodiscard]] auto end() const noexcept {
        return m_cookie.end();
    }

    void cookieIs(Cookie const& cookie) {
        m_cookie[cookie.name()] = cookie;
    }

    static inline const std::string HOST ;
    static inline const std::string CONTENT_LENGTH ;
    static inline const std::string ACCEPT_ENCODING ;
    static inline const std::string CONNECTION ;

private:
    Map m_cookie;
};

}  // namespace http


  // namespace http
