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
			host->type = HTTP_HEADER_HOST;
			host->value = value;
			parsed_header = static_cast<HttpHeader *>(host);
		}
		else if (name == "accept")
		{
			HttpHeaderAccept *accept = new HttpHeaderAccept();
			accept->type = HTTP_HEADER_ACCEPT;
			accept->list.push_back(value);
			parsed_header = static_cast<HttpHeader *>(accept);
		}
		else
		{
			HttpHeaderUnknown *unknown_header = new HttpHeaderUnknown();
			unknown_header->type = HTTP_HEADER_UNKNOWN;
			unknown_header->value = value;
			parsed_header = static_cast<HttpHeader *>(unknown_header);
		}

		return parsed_header;
	}
}
