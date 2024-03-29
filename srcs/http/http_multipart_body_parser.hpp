#pragma once

#include <string>

#include "http_multipart_body.hpp"
#include "http_header_map.hpp"

#define MIN_N_CHARS_MULTIPART_BODY 16

namespace ws
{
    class HttpMultipartBodyParser
    {
        public:
            HttpMultipartBodyParser(const std::string &body_content, const std::string &boundary);
            bool is_valid();
            HttpMultipartBody parse();

        private:
            HttpMultipartBody body_;
            std::string content_;
            std::string boundary_;
            bool valid_;

    };
}
