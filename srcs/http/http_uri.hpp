#pragma once

#include <string>
#include <map>

namespace ws
{
    struct HttpUri
    {
        typedef std::map<std::string, std::string>::iterator params_iterator;
        typedef std::map<std::string, std::string>::const_iterator params_const_iterator;
        HttpUri();
        std::string absolute_path();

        std::string schema;
        std::string host;
        size_t port;
        std::string path;
        std::string query;
        bool is_absolute;
    };

    bool operator==(const HttpUri &lhs, const HttpUri &rhs);
    bool operator!=(const HttpUri &lhs, const HttpUri &rhs);
}
