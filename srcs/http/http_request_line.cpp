#include "http_request_line.hpp"

namespace ws
{
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

	bool operator==(const HttpRequestLine &lhs, const HttpRequestLine &rhs)
    {
        if (lhs.method != rhs.method)
            return false;
        if (lhs.uri != rhs.uri )
            return false;
        if (lhs.http_version != rhs.http_version)
            return false;
        return true;
    }

    bool operator!=(const HttpRequestLine &lhs, const HttpRequestLine &rhs)
    {
        return !(lhs == rhs);
    }
}
