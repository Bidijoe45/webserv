#include <string>
#include <stdexcept>
#include <iostream>
#include <cstdlib>

#include "http_headers.hpp"
#include "string_utils.hpp"

namespace ws
{
	HttpHeaderContentLength::HttpHeaderContentLength()
	{
		this->type = HTTP_HEADER_CONTENT_LENGTH;
	}

	HttpHeaderContentLength::~HttpHeaderContentLength() {}
	
	HttpHeaderContentLength::HttpHeaderContentLength(const HttpHeaderContentLength &src)
	{
		this->type = src.type;
		this->value = src.value;
		this->content_length = src.content_length;
	}

	void HttpHeaderContentLength::parse_value()
	{
		if (this->value.size() == 0 || is_string_digit(this->value, this->value.size()) == false)
		{
			this->is_valid = false;
			std::cout << "Content Length Header: invalid content-length chars" << std::endl;
			return;
		}
		this->content_length = strtoul(this->value.c_str(), NULL, 10);
	}

	void HttpHeaderContentLength::set_value(const std::string &value)
	{
		this->value = value;
		this->parse_value();
	}

	void HttpHeaderContentLength::set_value(const size_t &value)
	{
		this->value = ul_to_string(value);
		this->content_length = value;
	}

}
