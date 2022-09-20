#pragma once

#include <string>
#include "../http/http_uri.hpp"

namespace ws
{
struct Redirect {
	Redirect();

	size_t code;
	HttpUri to;
};

bool operator==(const Redirect &lhs, const Redirect &rhs);

} // namespace ws
