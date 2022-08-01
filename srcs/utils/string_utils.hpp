#pragma once

#include <string>
#include <vector>

namespace ws
{
	bool is_string_printable(std::string str, size_t len);
	std::vector<std::string> string_split(const std::string &str, const std::string &delim);
}
