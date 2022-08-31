#include "cgi_settings.hpp"

namespace ws
{
	bool operator==(const CGISettings &lhs, const CGISettings &rhs)
	{
		if (lhs.extension == rhs.extension && lhs.executable == rhs.executable)
			return true;
		return false;
	}
}
