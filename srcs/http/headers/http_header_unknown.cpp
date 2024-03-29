#include "http_headers.hpp"

namespace ws
{
	HttpHeaderUnknown::HttpHeaderUnknown()
	{
		this->type = HTTP_HEADER_UNKNOWN;
	}

	HttpHeaderUnknown::~HttpHeaderUnknown() {}

	HttpHeaderUnknown::HttpHeaderUnknown(const HttpHeaderUnknown &src)
	{
		this->type = src.type;
		this->value = src.value;
	}

	void HttpHeaderUnknown::set_value(const std::string &value)
	{
		this->value = value;
	}

	void HttpHeaderUnknown::parse_value() {}

}
