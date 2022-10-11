#include <iostream>

#include "http_uri_parser_tests_utils.hpp"

namespace ws_tester
{
    void print_http_uri(const ws::HttpUri &uri)
    {
        std::cout << "-- HttpUri --" << std::endl;
        std::cout << "host: " << uri.host << std::endl;
        std::cout << "port: " << uri.port << std::endl;
        std::cout << "path: " << uri.path << std::endl;
        std::cout << "is_absolute: " << ((uri.is_absolute) ? "true" : "false") << std::endl;
        std::cout << "-- --" << std::endl;

    }
}
