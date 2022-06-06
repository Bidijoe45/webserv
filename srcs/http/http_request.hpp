#pragma once

#include <string>
#include <map>

#include "../server/data_buffer.hpp"

namespace ws
{

enum HTTP_METHOD {
	GET, POST, DELETE, INVALID_METHOD
};

struct HttpRequest {
	typedef std::map<std::string, std::string>::iterator headers_iterator;

	HTTP_METHOD method;
	std::string url;
	std::map<std::string, std::string> headers;
	std::string body;
};

} // namespace ws