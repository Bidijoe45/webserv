#pragma once
#include <string>

#include "http_header_map.hpp"

namespace ws
{

    struct HttpResponse
    {
        HttpResponse();
        std::string to_string();
        void generate_content_type_header(std::string content_type);
        void generate_content_length_header();
        std::string http_version;
        unsigned int status_code;
        std::string status_msg;
        HttpHeaderMap headers;
        std::string body;
    };
    
}

