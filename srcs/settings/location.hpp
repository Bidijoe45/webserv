#pragma once

#include <string>
#include <vector>

#include "../http/http_request.hpp"
#include "redirect.hpp"
#include "cgi_settings.hpp"

namespace ws {

struct Location {
    Location();
    std::string path;
    std::string root;
    std::string index;
    std::vector<HTTP_METHOD> methods;
    Redirect redirect;
	bool autoindex;
    std::vector<CGISettings> cgis;
    std::string upload_dir;
};

bool operator==(const Location &lhs, const Location &rhs);

}  // namespace ws
