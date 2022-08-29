#pragma once

#include <string>
#include <vector>

#include "../http/http_request.hpp"
#include "rewrite.hpp"
#include "cgi_settings.hpp"

namespace ws {

struct Location {
    std::string path;
    std::string root;
    std::string index;
    std::vector<HTTP_METHOD> methods;
    std::vector<Rewrite> rewrites;
	bool autoindex;
    std::vector<CGISettings> cgis;
};

bool operator==(const Location &lhs, const Location &rhs);

}  // namespace ws
