#pragma once

#include <string>

namespace ws
{
struct Rewrite {
	std::string from;
	std::string to;
	bool permanent;
};

bool operator==(const Rewrite &lhs, const Rewrite &rhs);

} // namespace ws
