#include "http_request.hpp"

namespace ws
{
	HttpRequest::HttpRequest() : error(HTTP_REQUEST_NO_ERROR), body(NULL) {}

	HttpRequest::HttpRequest(const HttpRequest &request)
	{
		if (this->body != NULL)
		{
			if (this->body_type == HTTP_SIMPLE_BODY)
				this->body = new HttpBody(request.body);

		}
	}

	HttpRequest::~HttpRequest() {
		if (body != NULL)
			delete body;
	}

	bool HttpRequest::is_valid()
	{
		return this->error == HTTP_REQUEST_NO_ERROR;
	}

	bool operator==(const HttpRequest &lhs, const HttpRequest &rhs) {
		
		if (lhs.method != rhs.method)
			return false;
		if (lhs.uri != rhs.uri)
			return false;
		if (lhs.http_version != rhs.http_version)
			return false;
		if (lhs.headers != rhs.headers)
			return false;
		if (lhs.error != rhs.error)
			return false;

		return true;
	}
} // namespace ws


