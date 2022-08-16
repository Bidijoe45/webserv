#include "http_header.hpp"

namespace ws
{
	HttpHeaderAccept::HttpHeaderAccept()
	{
		this->type = HTTP_HEADER_ACCEPT;
		this->is_list_based = true;
	}

	HttpHeaderAccept::~HttpHeaderAccept() {}

	HttpHeaderAccept::HttpHeaderAccept(const HttpHeaderAccept &src)
	{
		this->type = src.type;
		this->value = src.value;
	}

	void HttpHeaderAccept::parse_value() {}

	void HttpHeaderAccept::set_value(const std::string &value)
	{
		this->value = value;
		this->parse_value();
	}
}