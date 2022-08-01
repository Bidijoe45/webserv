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
}
