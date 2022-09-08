#include "http_header.hpp"
#include "headers/http_headers.hpp"

namespace ws
{
	HttpHeader::~HttpHeader() {}

	HttpHeaderListBased::~HttpHeaderListBased() {}

	HttpHeaderSingleton::~HttpHeaderSingleton() {}

	HTTP_HEADER_TYPE HttpHeader::resolve_header_name(const std::string &name)
	{
		if (name == "host")
			return HTTP_HEADER_HOST;
		else if (name == "accept")
			return HTTP_HEADER_ACCEPT;
		else if (name == "content-length")
			return HTTP_HEADER_CONTENT_LENGTH;
		else if (name == "content-disposition")
			return HTTP_HEADER_CONTENT_DISPOSITION;
		else
			return HTTP_HEADER_UNKNOWN;
	}

	std::string HttpHeader::header_type_to_string(const HTTP_HEADER_TYPE &type)
	{
		switch (type)
		{
			case HTTP_HEADER_ACCEPT:
				return "accept";
			case HTTP_HEADER_HOST:
				return "host";
			case HTTP_HEADER_CONTENT_LENGTH:
				return "content-length";
			case HTTP_HEADER_CONTENT_DISPOSITION:
				return "content-disposition";
			default:
				return "unknown";
		}
	}

	HttpHeader *HttpHeader::alloc_new_header(HttpHeader *header)
	{
		switch (header->type)
		{
			case HTTP_HEADER_ACCEPT:
				return new HttpHeaderAccept(*static_cast<const HttpHeaderAccept*>(header));
			case HTTP_HEADER_HOST:
				return new HttpHeaderHost(*static_cast<const HttpHeaderHost*>(header));
			case HTTP_HEADER_CONTENT_LENGTH:
				return new HttpHeaderContentLength(*static_cast<const HttpHeaderContentLength*>(header));
			case HTTP_HEADER_CONTENT_DISPOSITION:
				return new HttpHeaderContentDisposition(*static_cast<const HttpHeaderContentDisposition*>(header));
			default:
				return new HttpHeaderUnknown(*static_cast<const HttpHeaderUnknown*>(header));
		}
	}

	HttpHeader *HttpHeader::alloc_new_header(const HTTP_HEADER_TYPE &type)
	{
		switch (type)
		{
			case HTTP_HEADER_ACCEPT:
				return new HttpHeaderAccept();
			case HTTP_HEADER_HOST:
				return new HttpHeaderHost();
			case HTTP_HEADER_CONTENT_LENGTH:
				return new HttpHeaderContentLength();
			case HTTP_HEADER_CONTENT_DISPOSITION:
				return new HttpHeaderContentDisposition();
			default:
				return new HttpHeaderUnknown();
		}
	}
}
