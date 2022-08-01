#include "http_header.hpp"

namespace ws
{
	HttpHeader::~HttpHeader() {}

	HttpHeaderSingleton::~HttpHeaderSingleton() {}

	HttpHeaderListBased::~HttpHeaderListBased() {}

	const std::string HttpHeaderSingleton::get_header_value_string() const
	{
		return this->value;
	}

	const std::string HttpHeaderListBased::get_header_value_string() const
	{
		std::string str;
		std::vector<std::string>::const_iterator it;

		for (it = this->list.begin(); it != this->list.end(); it++)
			str.append(*it);
		return str;
	}

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
