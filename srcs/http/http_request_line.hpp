#pragma once

#include <string>

#include "http_uri.hpp"

namespace ws
{
    enum HTTP_METHOD {
	    HTTP_METHOD_GET,
	    HTTP_METHOD_POST,
	    HTTP_METHOD_DELETE,
	    HTTP_METHOD_INVALID
    };

    struct HttpRequestLine
    {
		HTTP_METHOD method;
		HttpUri uri;
		std::string http_version;
    };

    bool operator==(const HttpRequestLine &lhs, const HttpRequestLine &rhs);
    bool operator!=(const HttpRequestLine &lhs, const HttpRequestLine &rhs);
}
