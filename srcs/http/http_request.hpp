#pragma once

#include <string>
#include <map>

#include "../server/data_buffer.hpp"
#include "http_header.hpp"
#include "http_header_map.hpp"
#include "http_request_line.hpp"

namespace ws
{

struct HttpRequest {
	public:
		typedef std::map<std::string, HttpHeader*>::iterator headers_iterator;
		HttpRequest();

		HttpRequestLine request_line;
		HttpHeaderMap headers;
		std::string body;
		bool is_valid;
};

bool operator==(const HttpRequest &lhs, const HttpRequest &rhs); 

} // namespace ws
