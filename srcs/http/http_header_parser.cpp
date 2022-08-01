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
			host->value = value;
			parsed_header = static_cast<HttpHeader *>(host);
		}
		else if (name == "accept")
		{
			HttpHeaderAccept *accept = new HttpHeaderAccept();
			accept->list.push_back(value);
			parsed_header = static_cast<HttpHeader *>(accept);
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
