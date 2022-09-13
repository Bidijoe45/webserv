#pragma once

#include <string>

namespace ws
{
    struct CGISettings {
        std::string extension;
        std::string executable;
    };

	bool operator==(const CGISettings &lhs, const CGISettings &rhs);
}
