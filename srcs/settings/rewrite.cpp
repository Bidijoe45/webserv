#include "rewrite.hpp"

namespace ws
{
	bool operator==(const Rewrite &lhs, const Rewrite &rhs) {
		if (lhs.from == rhs.from && lhs.to == rhs.to && lhs.permanent == rhs.permanent) {
			return true;
		}
		return false;
	}
} // namespace ws
