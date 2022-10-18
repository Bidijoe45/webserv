#include <string>
#include <iostream>

#include "http_headers.hpp"
#include "string_utils.hpp"

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
			this->host = this->value.substr(0, colon_pos);
			this->host = string_to_lower(this->host);

			std::string port = this->value.substr(colon_pos + 1);
			if (is_string_digit(port, port.size()) == false)
			{
				this->is_valid = false;
				std::cout << "Host Header: port must be digits" << std::endl;
				return;
			}
			if (port != "")
				this->port = std::stoul(port);
		}
		else
			this->host = this->value.substr(0);

		if (this->host.size() == 0)
		{
			this->is_valid = false;
			std::cout << "Host Header: host is empty" << std::endl;
		}
	}

	void HttpHeaderHost::set_value(const std::string &value)
	{
		this->value = value;
		this->parse_value();
	}
}
