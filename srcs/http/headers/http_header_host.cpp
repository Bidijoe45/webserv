#include <string>
#include <iostream>

#include "http_headers.hpp"

namespace ws
{
	HttpHeaderHost::HttpHeaderHost()
	{
		this->type = HTTP_HEADER_HOST;
		this->port = -1;
	}

	HttpHeaderHost::~HttpHeaderHost() {}
	
	HttpHeaderHost::HttpHeaderHost(const HttpHeaderHost &src)
	{
		this->type = src.type;
		this->value = src.value;
		this->host = src.host;
		this->port = src.port;
	}

	void HttpHeaderHost::parse_value()
	{
		size_t colon_pos = this->value.find_first_of(':');

		if (colon_pos != std::string::npos)
		{
			this->host = this->value.substr(0, colon_pos); // case insensitive? hex?
			this->port = std::stoul(this->value.substr(colon_pos + 1));
		}
		else
			this->host = this->value.substr(0);
	}

	void HttpHeaderHost::set_value(const std::string &value)
	{
		this->value = value;
		this->parse_value();
	}
}
