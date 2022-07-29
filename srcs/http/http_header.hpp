#pragma once

#include <string>
#include <vector>

namespace ws
{

	enum HTTP_HEADER_TYPE
	{
		HTTP_HEADER_UNKNOWN,
		HTTP_HEADER_HOST,
		HTTP_HEADER_ACCEPT
	};

	struct HttpHeader
	{
		virtual ~HttpHeader();
		virtual const std::string get_header_value_string() const = 0;
		HTTP_HEADER_TYPE type;
	};

	struct HttpHeaderUnknown : public HttpHeader
	{
		HttpHeaderUnknown();
		~HttpHeaderUnknown();
		HttpHeaderUnknown(const HttpHeaderUnknown &src);
		const std::string get_header_value_string() const;
		std::string value;
	};

	struct HttpHeaderAccept : public HttpHeader
	{
		HttpHeaderAccept();
		~HttpHeaderAccept();
		HttpHeaderAccept(const HttpHeaderAccept &src);
		const std::string get_header_value_string() const;
		std::vector<std::string> list;
	};

	struct HttpHeaderHost : public HttpHeader
	{
		HttpHeaderHost();
		~HttpHeaderHost();
		HttpHeaderHost(const HttpHeaderHost &src);
		const std::string get_header_value_string() const;
		std::string value;
	};
}
