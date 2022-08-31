#include "location.hpp"

#include <iostream>

namespace ws
{
	bool operator==(const Location &lhs, const Location &rhs) {
		if (lhs.path == rhs.path && lhs.root == rhs.root && lhs.index == rhs.index
			&& lhs.methods == rhs.methods && lhs.rewrites == rhs.rewrites
			&& lhs.autoindex == rhs.autoindex && lhs.cgis == rhs.cgis)
		{
			return true;
		}
		return false;
	}
} // namespace ws


