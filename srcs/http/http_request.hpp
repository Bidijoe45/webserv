#pragma once

#include <string>
#include <map>

#include "data_buffer.hpp"
#include "http_uri.hpp"
#include "http_header.hpp"
#include "http_header_map.hpp"

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
		typedef std::map<std::string, HttpHeader*>::iterator headers_iterator;

		HttpRequest();
		~HttpRequest();
		bool is_valid();

		HTTP_METHOD method;
		HttpUri uri;
		std::string http_version;
		HttpHeaderMap headers;
		std::string body;
		HTTP_REQUEST_ERROR error;
};

bool operator==(const HttpRequest &lhs, const HttpRequest &rhs); 

} // namespace ws
