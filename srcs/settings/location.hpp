#pragma once

#include <string>
#include <vector>

#include "../http/http_request.hpp"
#include "rewrite.hpp"

namespace ws {

struct Location {
    std::string path;
    std::string root;
    std::string index;
    std::vector<HTTP_METHOD> methods;
    std::vector<Rewrite> rewrites;
	bool autoindex;
};

bool operator==(const Location &lhs, const Location &rhs);

}  // namespace ws
