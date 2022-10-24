#include "http_multipart_body.hpp"

namespace ws
{
    bool operator==(const HttpMultipartBodyPart &lhs, const HttpMultipartBodyPart &rhs)
    {
        return (lhs.header_map == rhs.header_map && lhs.content == rhs.content);
    }

    bool operator==(const HttpMultipartBody &lhs, const HttpMultipartBody &rhs)
    {
        return lhs.parts == rhs.parts;
    }
}
