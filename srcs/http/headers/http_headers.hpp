#pragma once

#include "../http_header.hpp"

namespace ws
{
	struct HttpHeaderAccept : public HttpHeaderListBased
	{
		HttpHeaderAccept();
		~HttpHeaderAccept();
		HttpHeaderAccept(const HttpHeaderAccept &src);
		void set_value(const std::string &value);
		HttpHeaderAccept &operator+=(const std::string &rhs);

		std::vector<std::string> list;

		private:
			void parse_value();
			void parse_added_value(const std::string &value);
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

	struct HttpHeaderContentDisposition : public HttpHeaderSingleton
	{
		HttpHeaderContentDisposition();
		~HttpHeaderContentDisposition();
		HttpHeaderContentDisposition(const HttpHeaderContentDisposition&src);
		void set_value(const std::string &value);

		std::string type;
		std::string name;
		std::string filename;

		private:
			void parse_value();
	};

	struct HttpHeaderUnknown : public HttpHeaderSingleton
	{
		HttpHeaderUnknown();
		~HttpHeaderUnknown();
		HttpHeaderUnknown(const HttpHeaderUnknown &src);
		void set_value(const std::string &value);
	};
}
