#include <string>

#include "http_header.hpp"
#include "string_utils.hpp"

namespace ws
{
	HttpHeaderContentLength::HttpHeaderContentLength()
	{
		this->type = HTTP_HEADER_CONTENT_LENGTH;
		this->is_list_based = false;
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
		this->content_length = std::stoul(this->value);
	}

	void HttpHeaderContentLength::set_value(const std::string &value)
	{
		this->value = value;
		this->parse_value();
	}

	void HttpHeaderContentLength::set_value(const size_t &value)
	{
		this->value = int_to_string(value);
		this->content_length = value;
	}

}