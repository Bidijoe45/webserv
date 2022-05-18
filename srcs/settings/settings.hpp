#pragma once

#include <string>
#include <vector>

#include "server_settings.hpp"

namespace ws {

struct Settings {
    std::vector<ServerSettings> servers;
};

bool operator==(const Settings &lhs, const Settings &rhs);

}  // namespace ws
