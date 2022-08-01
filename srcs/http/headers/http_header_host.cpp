#include "http_header.hpp"

namespace ws
{
	HttpHeaderHost::HttpHeaderHost()
	{
		this->type = HTTP_HEADER_HOST;
	}

	HttpHeaderHost::~HttpHeaderHost() {}
	
	HttpHeaderHost::HttpHeaderHost(const HttpHeaderHost &src)
	{
		this->type = src.type;
		this->value = src.value;
	}
}