#pragma once

#include <string>

#include "http_multipart_body.hpp"

namespace ws
{
    class HttpMultipartBodyParser
    {
        public: 
            HttpMultipartBodyParser(const std::string &body, const std::string &boundary);
            HttpMultipartBody *parse();

        private:
            std::string body_;
            std::string boundary_;
    };
}
