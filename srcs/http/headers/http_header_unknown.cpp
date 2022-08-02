#include "http_header.hpp"

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
}