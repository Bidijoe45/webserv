#pragma once

#include <string>
#include <map>

#include "../server/data_buffer.hpp"
#include "http_uri.hpp"

namespace ws
{

enum HTTP_METHOD {
	HTTP_METHOD_GET,
	HTTP_METHOD_POST,
	HTTP_METHOD_DELETE,
	HTTP_METHOD_INVALID
};

enum HTTP_REQUEST_ERROR {
	HTTP_REQUEST_NO_ERROR,
	HTTP_REQUEST_INVALID_METHOD,
	HTTP_REQUEST_INVALID_URI,
	HTTP_REQUEST_INVALID_HEADER,
	HTTP_REQUEST_INVALID_VERSION
};

struct HttpRequest {
	public:
		typedef std::map<std::string, std::string>::iterator headers_iterator;

		HttpRequest();
		bool is_valid();

		HTTP_METHOD method;
		HttpUri uri;
		std::string http_version;
		std::map<std::string, std::string> headers;
		std::string body;
		HTTP_REQUEST_ERROR error;
};

bool operator==(const HttpRequest &lhs, const HttpRequest &rhs); 

} // namespace ws
