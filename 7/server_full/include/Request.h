//
// Created by Jollu Emil.
//

#include <string>

#include "Headers.h"
#include "Uri.h"

namespace http {
class Request {
public:
    enum Method { GET, HEAD, POST, PUT, DELETE, TRACE, CONNECT };

public:  // Getters
    Method method() const {
        return m_method;
    }
    Uri const& uri() const {
        return m_uri;
    }
    std::string const& path() const {
        return m_uri.path();
    }

    std::string const& data() const {
        return m_data;
    }
    std::string const header(std::string const& name) const;
    Headers const& headers() const {
        return m_headers;
    }

public:  // Setters
    void methodsIs(Method method);
    void uriIs(Uri const& path);
    void dataIs(std::string const& data);
    void headers(std::string const& name, std::string const& value);

private:
    Method m_method = GET;
    Uri m_uri;
    std::string m_data;
    Headers m_headers;
};
}  // namespace http