#pragma once

#include <string>
#include "http_header_map.hpp"

namespace ws
{
    struct HttpMultipartBodyPart
    {
        HttpHeaderMap header_map;
        std::string content;
    };

    struct HttpMultipartBody
    {
        std::vector<HttpMultipartBodyPart> parts;
    };

    bool operator==(const HttpMultipartBodyPart &lhs, const HttpMultipartBodyPart &rhs);
    bool operator==(const HttpMultipartBody &lhs, const HttpMultipartBody &rhs);
}
