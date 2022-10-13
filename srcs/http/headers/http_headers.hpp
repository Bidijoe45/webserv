#pragma once

#include <map>

#include "../http_header.hpp"
#include "../http_uri.hpp"

namespace ws
{
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

		std::string content_type;
		std::string name;
		std::string filename;

		private:
			void parse_value();
	};
	
	struct HttpHeaderContentType : public HttpHeaderSingleton
	{
		HttpHeaderContentType();
		~HttpHeaderContentType();
		HttpHeaderContentType(const HttpHeaderContentType&src);
		void set_value(const std::string &value);

		std::string content_type;
		std::map<std::string, std::string> parameters;

		private:
			void parse_value();
	};

	struct HttpHeaderCGIStatus : public HttpHeaderSingleton
	{
		HttpHeaderCGIStatus();
		~HttpHeaderCGIStatus();
		HttpHeaderCGIStatus(const HttpHeaderCGIStatus &src);
		void set_value(const std::string &value);

		unsigned int status_code;
		std::string reason_phrase;

		private:
			void parse_value();
	};

	struct HttpHeaderLocation : public HttpHeaderSingleton
	{
		HttpHeaderLocation();
		~HttpHeaderLocation();
		HttpHeaderLocation(const HttpHeaderLocation &src);
		void set_value(const std::string &value);

		HttpUri uri;

		private:
			void parse_value();
	};

	struct HttpHeaderTransferEncoding : public HttpHeaderListBased
	{
		HttpHeaderTransferEncoding();
		~HttpHeaderTransferEncoding();
		HttpHeaderTransferEncoding(const HttpHeaderTransferEncoding &src);
		void set_value(const std::string &value);
		HttpHeaderTransferEncoding &operator+=(const std::string &rhs);

		std::vector<std::string> codings;

		private:
			void parse_value();
			void parse_value(const std::string &val);
	};

	struct HttpHeaderConnection : public HttpHeaderListBased
	{
		HttpHeaderConnection();
		~HttpHeaderConnection();
		HttpHeaderConnection(const HttpHeaderConnection &src);
		void set_value(const std::string &value);
		HttpHeaderConnection &operator+=(const std::string &rhs);

		std::vector<std::string> options;

		private:
			void parse_value();
			void parse_value(const std::string &val);
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
}
