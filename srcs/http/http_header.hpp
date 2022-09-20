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
		HTTP_HEADER_CONTENT_LENGTH,
		HTTP_HEADER_CONTENT_DISPOSITION,
		HTTP_HEADER_CONTENT_TYPE,
		HTTP_HEADER_CGI_STATUS
	};

	struct HttpHeader
	{
		virtual ~HttpHeader() = 0;
		virtual void set_value(const std::string &value) = 0;
		static HttpHeader *alloc_new_header(const HTTP_HEADER_TYPE &type);
		static HttpHeader *alloc_new_header(HttpHeader *header_type);
		static HTTP_HEADER_TYPE resolve_header_name(const std::string &name);
		static std::string header_type_to_string(const HTTP_HEADER_TYPE &type);

		HTTP_HEADER_TYPE	type;
		std::string			value;
	};

	struct HttpHeaderSingleton : public HttpHeader
	{
		virtual ~HttpHeaderSingleton() = 0;
	};

	struct HttpHeaderListBased : public HttpHeader
	{
		virtual ~HttpHeaderListBased() = 0;
		virtual HttpHeaderListBased &operator+=(const std::string &rhs) = 0;

		private:
			virtual void parse_added_value(const std::string &value) = 0;
	};
}
