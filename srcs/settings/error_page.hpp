#pragma once

#include <string>
namespace ws
{
struct ErrorPage {
    size_t code;
    std::string path;
};

bool operator==(const ErrorPage &lhs, const ErrorPage &rhs);

} // namespace ws


