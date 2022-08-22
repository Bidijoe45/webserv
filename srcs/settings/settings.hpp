#pragma once

#include <string>
#include <vector>

#include "server_settings.hpp"
#include "../http/http_request.hpp"

namespace ws {

struct Settings {
    std::vector<ServerSettings> servers;
    ServerSettings resolve_settings_hostname(const HttpRequest &request, int port);
};

bool operator==(const Settings &lhs, const Settings &rhs);

}  // namespace ws
