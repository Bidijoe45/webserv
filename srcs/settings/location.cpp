#include "location.hpp"

#include <iostream>

namespace ws
{

	Location::Location() : autoindex(false) {}

	bool operator==(const Location &lhs, const Location &rhs) {
		if (lhs.path == rhs.path && lhs.root == rhs.root && lhs.index == rhs.index
			&& lhs.methods == rhs.methods && lhs.rewrites == rhs.rewrites
			&& lhs.autoindex == rhs.autoindex)
		{
			return true;
		}
		return false;
	}
} // namespace ws


