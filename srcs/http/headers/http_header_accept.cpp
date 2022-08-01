#include "http_header.hpp"

namespace ws
{
	HttpHeaderAccept::HttpHeaderAccept()
	{
		this->type = HTTP_HEADER_ACCEPT;
	}

	HttpHeaderAccept::~HttpHeaderAccept() {}

	HttpHeaderAccept::HttpHeaderAccept(const HttpHeaderAccept &src)
	{
		this->type = src.type;
		this->list = src.list;
	}
}