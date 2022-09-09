#include "http_body.hpp"

namespace ws
{
    HttpBody::HttpBody(const std::string &content) : content(content) {}
    HttpBody::HttpBody(const HttpBody *http_body)
    {
        this->content = http_body->content;
    }
}
