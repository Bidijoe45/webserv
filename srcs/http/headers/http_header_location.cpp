#include <string>
#include <iostream>

#include "http_headers.hpp"
#include "../http_uri_parser.cpp"

namespace ws
{
	HttpHeaderLocation::HttpHeaderLocation()
	{
		this->type = HTTP_HEADER_LOCATION;
	}

	HttpHeaderLocation::~HttpHeaderLocation() {}
	
	HttpHeaderLocation::HttpHeaderLocation(const HttpHeaderLocation &src)
	{
		this->type = src.type;
		this->value = src.value;
		this->uri = src.uri;
	}

	void HttpHeaderLocation::parse_value()
	{
		this->uri = HttpUriParser(this->value).parse();
	}

	void HttpHeaderLocation::set_value(const std::string &value)
	{
		this->value = value;
		this->parse_value();
	}
}
