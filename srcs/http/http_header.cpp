#include "http_header.hpp"

namespace ws
{
	HttpHeader::~HttpHeader() {}

	HttpHeaderUnknown::HttpHeaderUnknown() {}

	HttpHeaderUnknown::~HttpHeaderUnknown() {}

	HttpHeaderUnknown::HttpHeaderUnknown(const HttpHeaderUnknown &src)
	{
		this->type = src.type;
		this->value = src.value;
	}

	const std::string HttpHeaderUnknown::get_header_value_string() const
	{
		return this->value;
	}

	HttpHeaderAccept::HttpHeaderAccept() {}

	HttpHeaderAccept::~HttpHeaderAccept() {}

	HttpHeaderAccept::HttpHeaderAccept(const HttpHeaderAccept &src)
	{
		this->type = src.type;
		this->list = src.list;
	}

	const std::string HttpHeaderAccept::get_header_value_string() const
	{
		std::string str;
		std::vector<std::string>::const_iterator it;

		for (it = this->list.begin(); it != this->list.end(); it++)
			str.append(*it);
		return str;
	}

	HttpHeaderHost::HttpHeaderHost() {}

	HttpHeaderHost::~HttpHeaderHost() {}
	
	HttpHeaderHost::HttpHeaderHost(const HttpHeaderHost &src)
	{
		this->type = src.type;
		this->value = src.value;
	}

	const std::string HttpHeaderHost::get_header_value_string() const
	{
		return this->value;
	}

}
