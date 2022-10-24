#include "server_settings.hpp"

namespace ws
{
	bool operator==(const ServerSettings &lhs, const ServerSettings &rhs) {
		if (lhs.port == rhs.port && lhs.server_name == rhs.server_name
			&& lhs.client_max_body_size == rhs.client_max_body_size
			&& lhs.error_pages == rhs.error_pages
			&& lhs.locations == rhs.locations) {
				return true;
		}

		return false;
	}

} // namespace ws



