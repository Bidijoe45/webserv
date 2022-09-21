#include "location.hpp"

#include <iostream>

namespace ws
{

	Location::Location() : autoindex(false) {}

	bool operator==(const Location &lhs, const Location &rhs) {
		if (lhs.path == rhs.path && lhs.root == rhs.root && lhs.index == rhs.index
			&& lhs.methods == rhs.methods && lhs.redirect == rhs.redirect
			&& lhs.autoindex == rhs.autoindex && lhs.cgis == rhs.cgis)
		{
			return true;
		}
		return false;
	}
} // namespace ws


