#pragma once

#include <string>
#include <map>

#include "../server/data_buffer.hpp"

namespace ws
{

enum HTTP_METHOD {
	HTTP_METHOD_GET,
	HTTP_METHOD_POST,
	HTTP_METHOD_DELETE,
	HTTP_METHOD_INVALID
};

enum HTTP_REQUEST_ERROR {
	HTTP_REQUEST_NO_ERROR,
	HTTP_REQUEST_INVALID_METHOD,
	HTTP_REQUEST_INVALID_URI,
	HTTP_REQUEST_INVALID_HEADER,
	HTTP_REQUEST_INVALID_VERSION
};

class HttpRequest {
	public:
		HttpRequest();
		typedef std::map<std::string, std::string>::iterator headers_iterator;
		bool set_method(const std::string &str);
		bool set_uri(const std::string &str);
		bool set_version(const std::string &str);
		bool set_header(const std::string &name, const std::vector<std::string> &value);
		bool set_body(const std::string &body);
		bool is_valid();
		HTTP_METHOD method;
		std::string uri;
		std::string http_version;
		std::map<std::string, std::string> headers;
		std::string body;
		HTTP_REQUEST_ERROR error;

	private:
		HTTP_REQUEST_ERROR validate_method();
		HTTP_REQUEST_ERROR validate_version();

};

bool operator==(const HttpRequest &lhs, const HttpRequest &rhs); 

} // namespace ws
