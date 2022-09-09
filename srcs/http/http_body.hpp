#pragma once

#include <string>

namespace ws
{
    typedef enum {
        HTTP_SIMPLE_BODY,
        HTTP_MULTIPART_BODY
    } HttpBodyType;

    struct HttpBody
    {
        HttpBody(const std::string &content);
        HttpBody(const HttpBody *http_body);
        std::string content;
    };
}
