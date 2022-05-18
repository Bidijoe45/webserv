#include "settings.hpp"

namespace ws
{
	bool operator==(const Settings &lhs, const Settings &rhs) {
		return lhs.servers == rhs.servers;
	}
} // namespace ws
