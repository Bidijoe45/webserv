#include "http_uri.hpp"

#include <iostream>
namespace ws
{
    HttpUri::HttpUri() : port(80), is_absolute(false) {}

    std::string HttpUri::absolute_path()
    {
        std::string abs_path;

        if (this->schema.size() > 0)
            abs_path += this->schema + "://";

        abs_path += this->path;

        if (this->query.size() > 0)
            abs_path += this->query;

        return abs_path;
    }

    bool operator==(const HttpUri &lhs, const HttpUri &rhs)
    {
        if (lhs.host != rhs.host)
            return false;
        if (lhs.port != rhs.port)
            return false;
        if (lhs.path != rhs.path)
            return false;
        if (lhs.query != rhs.query)
            return false;
        if (lhs.is_absolute != rhs.is_absolute)
            return false;
        if (lhs.params != rhs.params)
            return false;
        return true;
    }

    bool operator!=(const HttpUri &lhs, const HttpUri &rhs)
    {
        return !(lhs == rhs);
    }
}
