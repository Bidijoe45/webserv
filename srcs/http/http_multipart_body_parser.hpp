#pragma once

#include <string>

#include "http_multipart_body.hpp"
#include "http_header_map.hpp"

namespace ws
{
    class HttpMultipartBodyParser
    {
        public:
            HttpMultipartBodyParser(const std::string &body_content, const std::string &boundary);
            bool is_valid();
            HttpMultipartBody parse();

        private:
            std::string get_header_name(std::string line);
            std::string get_header_value(std::string line); 
            HttpHeaderMap parse_headers(std::string body);
            std::string content_;
            std::string boundary_;
            bool valid_;

    };
}
