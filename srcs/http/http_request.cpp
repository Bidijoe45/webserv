#include <string>
#include "http_request.hpp"

namespace ws
{
	HttpRequest::HttpRequest() : error(NO_ERROR) {}

	void HttpRequest::reset()
	{
		this->request_line = HttpRequestLine();
		this->headers.clear();
		this->body.clear();
		this->error = NO_ERROR;
	}

	bool operator==(const HttpRequest &lhs, const HttpRequest &rhs) {
		if (lhs.request_line != rhs.request_line)	
			return false;
		if (lhs.headers != rhs.headers)
			return false;
		if (lhs.body != rhs.body)
			return false;
		if (lhs.error != rhs.error)
			return false;

		return true;
	}
} // namespace ws


