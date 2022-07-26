#pragma once

#include <string>

namespace ws
{
    struct HttpUri
    {
        std::string host; //TODO: agregar mas campos que pueda tener la URI
    };

    bool operator==(const HttpUri &lhs, const HttpUri &rhs);
    bool operator!=(const HttpUri &lhs, const HttpUri &rhs);
}
