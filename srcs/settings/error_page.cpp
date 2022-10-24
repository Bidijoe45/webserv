#include "error_page.hpp"

namespace ws
{

	bool operator==(const ErrorPage &lhs, const ErrorPage &rhs) {
		if (lhs.code == rhs.code && lhs.path == rhs.path) {
			return true;
		}

		return false;
	}
	
} // namespace ws
