#include <string>

#include "http_header.hpp"

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
	}

	void HttpHeaderContentLength::parse_value()
	{
		this->content_length = std::stoul(this->value);
	}

	void HttpHeaderContentLength::set_value(const std::string &value)
	{
		this->value = value;
		this->parse_value();
	}

	void HttpHeaderContentLength::set_value(const size_t &value)
	{
		char str_length[10];
		sprintf(str_length, "%lu", value);
		this->value = str_length;
		this->content_length = value;
	}

}