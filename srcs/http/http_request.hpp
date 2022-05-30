#pragma once

#include <string>
#include <map>

#include "../server/data_buffer.hpp"

namespace ws
{

enum HTTP_METHOD {
	GET, POST, DELETE
};

struct HttpRequest {
	HTTP_METHOD method;
	std::string url;
	std::string version;
	std::map<std::string, std::string> headers;
	DataBuffer body;
};

} // namespace ws