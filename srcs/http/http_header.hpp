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

		HTTP_HEADER_TYPE	type;
	};

	struct HttpHeaderSingleton : public HttpHeader
	{
		virtual ~HttpHeaderSingleton();
		const std::string get_header_value_string() const;

		std::string value;
	};

	struct HttpHeaderListBased : public HttpHeader
	{
		virtual ~HttpHeaderListBased();
		const std::string get_header_value_string() const;

		std::vector<std::string> list;
	};

	struct HttpHeaderUnknown : public HttpHeaderSingleton
	{
		HttpHeaderUnknown();
		~HttpHeaderUnknown();
		HttpHeaderUnknown(const HttpHeaderUnknown &src);
	};

	struct HttpHeaderAccept : public HttpHeaderListBased
	{
		HttpHeaderAccept();
		~HttpHeaderAccept();
		HttpHeaderAccept(const HttpHeaderAccept &src);
	};

	struct HttpHeaderHost : public HttpHeaderSingleton
	{
		HttpHeaderHost();
		~HttpHeaderHost();
		HttpHeaderHost(const HttpHeaderHost &src);
	};
}
