#include <string>
#include "http_request.hpp"

namespace ws
{
	HttpRequest::HttpRequest() : is_valid(true) {}

	void HttpRequest::reset()
	{
		this->request_line = HttpRequestLine();
		this->headers.clear();
		this->body.clear();
		this->is_valid = true;
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


