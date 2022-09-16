#include <string>
#include "http_request.hpp"

namespace ws
{
	HttpRequest::HttpRequest() : is_valid(true) {}

	std::string HttpRequest::method_to_string() const
	{
		switch (this->method)
		{
			case HTTP_METHOD_GET:
				return "GET";
			case HTTP_METHOD_POST:
				return "POST";
			case HTTP_METHOD_DELETE:
				return "DELETE";
			default:
				return "INVALID";
		}
	}

	bool operator==(const HttpRequest &lhs, const HttpRequest &rhs) {
		if (lhs.request_line != rhs.request_line)	
			return false;
		if (lhs.headers != rhs.headers)
			return false;
		if (lhs.is_valid != rhs.is_valid)
			return false;

		return true;
	}
} // namespace ws


