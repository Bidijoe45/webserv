#pragma once

#include <string>
#include <vector>

namespace ws
{
	enum HTTP_HEADER_TYPE
	{
		HTTP_HEADER_UNKNOWN,
		HTTP_HEADER_HOST,
		HTTP_HEADER_ACCEPT,
		HTTP_HEADER_CONTENT_LENGTH
	};

	struct HttpHeader
	{
		virtual ~HttpHeader() = 0;
		virtual const std::string get_header_value_string() const = 0;
		//static HTTP_HEADER_TYPE resolve_header_name(const std::string &name);

		HTTP_HEADER_TYPE	type;
	};

	struct HttpHeaderSingleton : public HttpHeader
	{
		virtual ~HttpHeaderSingleton() = 0;
		const std::string get_header_value_string() const;
		virtual void set_value(const std::string &value) = 0;

		std::string value;

		private:
			virtual void parse_value() = 0;
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

		void set_value(const std::string &value);

		private:
			void parse_value();
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

	struct HttpHeaderContentLength : public HttpHeaderSingleton
	{
		HttpHeaderContentLength();
		~HttpHeaderContentLength();
		HttpHeaderContentLength(const HttpHeaderContentLength &src);
		void set_value(const std::string &value);
		void set_value(const size_t &value);

		size_t content_length;

		private:
			void parse_value();
	};
}
