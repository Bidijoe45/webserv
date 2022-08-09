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
}