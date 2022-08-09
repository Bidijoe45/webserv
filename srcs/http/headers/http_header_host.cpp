#include <cstdlib>
#include "http_header.hpp"

namespace ws
{
	HttpHeaderHost::HttpHeaderHost()
	{
		this->type = HTTP_HEADER_HOST;
	}

	HttpHeaderHost::~HttpHeaderHost() {}
	
	HttpHeaderHost::HttpHeaderHost(const HttpHeaderHost &src)
	{
		this->type = src.type;
		this->value = src.value;
	}

	void HttpHeaderHost::parse_header_value()
	{
		size_t	colon_pos = this->value.find_first_of(':');

		this->host_uri = this->value.substr(0, colon_pos); // case insensitive? hex?
		this->port = atoi(this->value.substr(colon_pos).c_str());
	}
}
