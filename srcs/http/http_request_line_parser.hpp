#pragma once

#include <string>

#include "http_request_line.hpp"
#include "../server/data_buffer.hpp"

namespace ws
{
    class HttpRequestLineParser
    {
        public:
            HttpRequestLineParser(const std::string &line);
            HttpRequestLine parse();
            bool is_valid();

        private:
            void parse_method();
            void check_space();
            void parse_uri();
            void parse_version();
            void advance(size_t n);

            HttpRequestLine request_line_;
            std::string line_;
            size_t pos_;
            bool valid_;
    };
}
