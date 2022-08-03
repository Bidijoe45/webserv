#include "settings.hpp"

namespace ws
{
    ServerSettings Settings::resolve_settings(const HttpRequest &request)
    {
        ServerSettings server_settings;


		return server_settings;
    }

	bool operator==(const Settings &lhs, const Settings &rhs) {
		return lhs.servers == rhs.servers;
	}
} // namespace ws
