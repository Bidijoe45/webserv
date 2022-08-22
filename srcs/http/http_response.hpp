#pragma once
#include <string>

#include "http_header_map.hpp"

namespace ws
{

    struct HttpResponse
    {
        std::string http_version;
        unsigned int status_code;
        std::string status_msg;
        HttpHeaderMap headers;
        std::string body;
        std::string to_string();
    };
    
}

