#pragma once

#include <string>
#include <map>

#include "../server/data_buffer.hpp"
#include "http_header.hpp"
#include "http_header_map.hpp"
#include "http_request_line.hpp"

namespace ws
{

struct HttpRequest
{
	public:
		enum RequestError 
		{
			NO_ERROR,
			BAD_REQUEST,
			INVALID_VERSION,
			LENGTH_REQUIRED,
			NOT_IMPLEMENTED,
			BODY_TOO_LARGE,
			URI_TOO_LONG
		};

		typedef std::map<std::string, HttpHeader*>::iterator headers_iterator;
		HttpRequest();
		void reset();

		HttpRequestLine request_line;
		HttpHeaderMap headers;
		std::string body;
		RequestError error;
};

bool operator==(const HttpRequest &lhs, const HttpRequest &rhs); 

} // namespace ws
