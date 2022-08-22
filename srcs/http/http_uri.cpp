#include "http_uri.hpp"

namespace ws
{
    HttpUri::HttpUri() : port(80), is_absolute(false) {}

    bool operator==(const HttpUri &lhs, const HttpUri &rhs)
    {
        if (lhs.host != rhs.host)
            return false;
        if (lhs.port != rhs.port)
            return false;
        if (lhs.path != rhs.path)
            return false;
        return true;
    }

    bool operator!=(const HttpUri &lhs, const HttpUri &rhs)
    {
        return !(lhs == rhs);
    }
}
