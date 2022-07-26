#include "http_uri.hpp"

namespace ws
{
    bool operator==(const HttpUri &lhs, const HttpUri &rhs)
    {
        if (lhs.host != rhs.host)
            return false;
        return true;
    }

    bool operator!=(const HttpUri &lhs, const HttpUri &rhs)
    {
        return !(lhs == rhs);
    }
}
