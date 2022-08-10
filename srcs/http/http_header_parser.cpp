#include "http_header_parser.hpp"
#include "http_header.hpp"

namespace ws
{
	HttpHeaderParser::HttpHeaderParser() {}

	HttpHeader *HttpHeaderParser::parse(std::string name, std::string value)
	{
		HttpHeader	*parsed_header;

		if (name == "host")
		{
			HttpHeaderHost	*host = new HttpHeaderHost();
			host->set_value(value);
			parsed_header = static_cast<HttpHeader *>(host);
		}
		else if (name == "accept")
		{
			HttpHeaderAccept *accept = new HttpHeaderAccept();
			accept->list.push_back(value);
			parsed_header = static_cast<HttpHeader *>(accept);
		}
		else if (name == "content-length")
		{
			HttpHeaderContentLength *content_length = new HttpHeaderContentLength();
			content_length->set_value(value);
			parsed_header = static_cast<HttpHeader *>(content_length);
		}
		else
		{
			HttpHeaderUnknown *unknown_header = new HttpHeaderUnknown();
			unknown_header->value = value;
			parsed_header = static_cast<HttpHeader *>(unknown_header);
		}

		return parsed_header;
	}
}
