#pragma once

#include <string>
#include <vector>

#include "server_settings.hpp"
#include "../http/http_request.hpp"

namespace ws {

struct Settings {
    std::vector<ServerSettings> servers;
    ServerSettings resolve_settings(const HttpRequest &request);
};

bool operator==(const Settings &lhs, const Settings &rhs);

}  // namespace ws
