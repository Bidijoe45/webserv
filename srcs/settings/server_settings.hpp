#pragma once

#include <string>
#include <vector>

#include "error_page.hpp"
#include "location.hpp"

namespace ws {
struct ServerSettings {
    size_t port;
    std::string server_name;
    size_t client_max_body_size;
    std::vector<ErrorPage> error_pages;
    std::vector<Location> locations;
    //TODO: CGI
};

bool operator==(const ServerSettings &lhs, const ServerSettings &rhs);

}