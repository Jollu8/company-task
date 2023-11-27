//
// Created by Jollu Emil .
//

#include <string>

#include "Request.h"
#include "Response.h"
#include "Uri.h"

namespace http {
Response get(std::string const& path, std::string const& data = "");
Response post(std::string const& path, std::string const& data = "");
Response send(Request const& request);

std::string str(Request const& request);
}  // namespace http
