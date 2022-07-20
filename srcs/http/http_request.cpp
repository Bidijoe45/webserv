#include "http_request.hpp"

namespace ws
{
	void HttpRequest::set_method(const std::string &str)
	{
		if (str == "GET")
			this->method = HTTP_METHOD::GET;
		else if (str == "POST")
			this->method = HTTP_METHOD::POST;
		else if (str == "DELETE")
			this->method = HTTP_METHOD::DELETE;
		else
			this->method = HTTP_METHOD::INVALID_METHOD;
	}
} // namespace ws


