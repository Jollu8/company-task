//
// Created by Jollu Emil .
//

#pragma once

#include "Threadpool.h"

namespace http {
class Authority {
private:  // Fields
    std::string m_user;
    std::string m_host;
    std::uint16_t m_port;

public:  // Constructor
    Authority(std::string const &user, std::string const &host, std::uint16_t port);
    ~Authority();

public:  // Methods
    std::string const &user() const {
        return m_user;
    }
    std::string const &host() const {
        return m_host;
    }
    std::uint16_t port() const {
        return m_port;
    }

    void userIs(std::string const &user);
    void hostIs(std::string const &host);
    void portIs(std::uint16_t port);
};

class Uri {
private:  // Fields
    std::string m_scheme;
    Authority m_authority;
    std::string m_path;

public:  // Constructors
    Uri(char *const value);
    Uri(std::string const &value);
    Uri();

public:  // Getter
    std::string const &scheme() const {
        return m_scheme;
    }
    Authority const &authority() const {
        return m_authority;
    }
    std::string const &path() const {
        return m_path;
    }
    std::string const &host() const {
        return m_authority.host();
    }
    std::uint16_t port() const {
        return authority().port();
    }

public:  // Setters
    void schemeIs(std::string const &scheme);
    void authorityIs(Authority const &authority);
    void pathIs(std::string const &path);
};

}  // namespace http