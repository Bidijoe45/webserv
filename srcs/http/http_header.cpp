#include "http_header.hpp"

namespace ws
{
	HttpHeader::~HttpHeader() {}

	/*HTTP_HEADER_TYPE HttpHeader::resolve_header_name(const std::string &name)
	{
		if (name == "host")
			return HTTP_HEADER_HOST;
		else if (name == "accept")
			return HTTP_HEADER_ACCEPT;
		else if (name == "content-length")
			return HTTP_HEADER_CONTENT_LENGTH;
		else
			return HTTP_HEADER_UNKNOWN;
	}*/

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
