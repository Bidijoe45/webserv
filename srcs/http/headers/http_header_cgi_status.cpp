#include <cstdlib>
#include <string>
#include <iostream>

#include "http_headers.hpp"
#include "string_utils.hpp"

namespace ws
{
	HttpHeaderCGIStatus::HttpHeaderCGIStatus()
	{
		this->type = HTTP_HEADER_CGI_STATUS;
		this->status_code = -1;
	}

	HttpHeaderCGIStatus::~HttpHeaderCGIStatus() {}
	
	HttpHeaderCGIStatus::HttpHeaderCGIStatus(const HttpHeaderCGIStatus &src)
	{
		this->type = src.type;
		this->value = src.value;
		this->status_code = src.status_code;
		this->reason_phrase = src.reason_phrase;
	}

	void HttpHeaderCGIStatus::parse_value()
	{
		if (this->value.size() == 0)
			return;
		size_t pos = this->value.find(" ");	
		std::string code = this->value.substr(0, pos);
		if (is_string_digit(code, code.size()) == false || code.size() != 3)
		{
			this->is_valid = false;
			std::cout << "CGI Status Header: invalid status code" << std::endl;
		}
		this->status_code = strtoul(code.c_str(), NULL, 10);
		if (pos == std::string::npos)
			return;
		pos++;
		if (pos != std::string::npos)
			this->reason_phrase = this->value.substr(pos);
	}

	void HttpHeaderCGIStatus::set_value(const std::string &value)
	{
		this->value = value;
		this->parse_value();
	}
}
