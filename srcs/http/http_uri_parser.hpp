#pragma once

#include "http_uri.hpp"

namespace ws
{
    enum HTTP_URI_ERROR {
        HTTP_URI_VALID,
        HTTP_URI_BAD_PORT
    };

class HttpUriParser
    {
        public:
            HttpUriParser(const std::string &uri);
            void parse_scheme();
            void parse_host();
            void parse_port();
            void parse_path();
            void parse_query();
            HttpUri parse();
            HTTP_URI_ERROR get_error();
            bool uri_is_valid();
            bool is_abs();

        private:
            void decode();
            int parse_hex(const std::string &str); 
            HTTP_URI_ERROR error_;
            HttpUri uri_;
            std::string line_;
            size_t line_pos_;

    };
}
