#pragma once

#include <string>

namespace ws
{
struct Redirect {
	Redirect();

	size_t code;
	std::string to;
};

bool operator==(const Redirect &lhs, const Redirect &rhs);

} // namespace ws
