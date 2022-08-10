#pragma once

#include <string>
#include <vector>

#include "error_page.hpp"
#include "location.hpp"

namespace ws {
struct ServerSettings {
    typedef std::vector<Location>::iterator locations_it;
    typedef std::vector<Location>::const_iterator locations_cit;

    size_t port;
    std::string server_name;
    size_t client_max_body_size;
    std::vector<ErrorPage> error_pages;
    std::vector<Location> locations;
    //TODO: CGI
};

bool operator==(const ServerSettings &lhs, const ServerSettings &rhs);

}
