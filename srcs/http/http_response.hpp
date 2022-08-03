#pragma once
#include <string>

#include "http_header_map.hpp"

namespace ws
{

    struct HttpResponse
    {
        std::string version;
        size_t status_code;
        std::string status_msg;
        HttpHeaderMap headers;
        std::string body;
        std::string to_string();
    };
    
}

