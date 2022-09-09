#pragma once

#include <iostream>
#include <vector>

#include "http_body.hpp"
#include "http_header_map.hpp"

namespace ws
{
    struct HttpMultipartBodyPart
    {
        HttpHeaderMap headers;
        std::string body;
    };

    struct HttpMultipartBody : public HttpBody
    {
        HttpMultipartBody();
        std::vector<HttpMultipartBodyPart> parts;
    };
}
