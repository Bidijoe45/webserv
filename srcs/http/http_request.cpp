#include "http_request.hpp"

namespace ws
{
	HttpRequest::HttpRequest() : error(HTTP_REQUEST_NO_ERROR) {}

	void HttpRequest::set_method(const std::string &str)
	{
		if (str == "GET")
			this->method = HTTP_METHOD_GET;
		else if (str == "POST")
			this->method = HTTP_METHOD_POST;
		else if (str == "DELETE")
			this->method = HTTP_METHOD_DELETE;
		else
			this->method = HTTP_METHOD_INVALID;
	}

	HTTP_REQUEST_ERROR HttpRequest::validate_method()
	{
		return (this->method == HTTP_METHOD_INVALID) ? HTTP_REQUEST_INVALID_METHOD : HTTP_REQUEST_NO_ERROR;
	}

	HTTP_REQUEST_ERROR HttpRequest::validate_version()
	{
		return (this->http_version == "HTTP/1.1") ? HTTP_REQUEST_INVALID_VERSION : HTTP_REQUEST_NO_ERROR;
	}

	std::string HttpRequest::decode_url() {
		return this->url;
	}

	HTTP_REQUEST_ERROR HttpRequest::validate_url()
	{
		this->decode_url();

		std::string protocol = this->url.substr(0, 7);
		bool is_absolute = false;

		if (protocol == "http://")
			is_absolute = true;


		if (is_absolute)
		{

		}
		else
		{

		}

		return HTTP_REQUEST_NO_ERROR;
	}

	void HttpRequest::validate()
	{
		this->error = validate_method();
		this->error = validate_url();
		this->error = validate_version();
	}

	bool HttpRequest::is_valid()
	{
		return this->error == HTTP_REQUEST_NO_ERROR;
	}

	bool operator==(const HttpRequest &lhs, const HttpRequest &rhs) {
		
		if (lhs.method != rhs.method)
			return false;
		if (lhs.url != rhs.url)
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


