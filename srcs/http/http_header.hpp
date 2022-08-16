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
		virtual void set_value(const std::string &value) = 0;
		static HttpHeader *alloc_new_header(HTTP_HEADER_TYPE type);
		static HttpHeader *alloc_new_header(HttpHeader *header_type);
		static HTTP_HEADER_TYPE resolve_header_name(const std::string &name);
		static std::string header_type_to_string(HTTP_HEADER_TYPE type);

		HTTP_HEADER_TYPE	type;
		std::string			value;
		bool				is_list_based;
	};

	struct HttpHeaderUnknown : public HttpHeader
	{
		HttpHeaderUnknown();
		~HttpHeaderUnknown();
		HttpHeaderUnknown(const HttpHeaderUnknown &src);
		void set_value(const std::string &value);

		private:
			void parse_value();
	};

	struct HttpHeaderAccept : public HttpHeader
	{
		HttpHeaderAccept();
		~HttpHeaderAccept();
		HttpHeaderAccept(const HttpHeaderAccept &src);
		void set_value(const std::string &value);

		std::vector<std::string> list;

		private:
			void parse_value();
	};

	struct HttpHeaderHost : public HttpHeader
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

	struct HttpHeaderContentLength : public HttpHeader
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
