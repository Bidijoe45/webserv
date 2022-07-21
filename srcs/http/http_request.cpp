#include "http_request.hpp"

namespace ws
{
	void HttpRequest::set_method(const std::string &str)
	{
		if (str == "GET")
			this->method = GET;
		else if (str == "POST")
			this->method = POST;
		else if (str == "DELETE")
			this->method = DELETE;
		else
			this->method = INVALID_METHOD;
	}
} // namespace ws


