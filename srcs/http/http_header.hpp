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
		virtual ~HttpHeader() = 0;
		virtual const std::string get_header_value_string() const = 0;

		HTTP_HEADER_TYPE	type;
	};

	struct HttpHeaderSingleton : public HttpHeader
	{
		virtual ~HttpHeaderSingleton() = 0;
		const std::string get_header_value_string() const;

		std::string value;
	};

	struct HttpHeaderListBased : public HttpHeader
	{
		virtual ~HttpHeaderListBased() = 0;
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
		void set_value(const std::string &value);

		std::string host;
		int port;

		private:
			void parse_value();
	};
}
