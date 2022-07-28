#pragma once

#include <string>

namespace ws
{
	bool is_string_printable(const std::string &str, size_t len);
	std::string string_to_lower(const std::string &std, size_t len);
	bool is_token(const std::string &str);
	bool is_obstext(const unsigned char &c);
}
