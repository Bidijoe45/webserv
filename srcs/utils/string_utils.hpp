#pragma once

#include <string>
#include <vector>

namespace ws
{
	bool is_string_printable(const std::string &str, size_t len);
	std::string string_to_lower(const std::string &std, size_t len);
	bool is_token(const std::string &str);
	bool is_obstext(const unsigned char &c);
	std::vector<std::string> string_split(const std::string &str, const std::string &delim);
	std::string compress_slash(const std::string &str);
	std::string int_to_string(const int n);
	std::string string_trim(const std::string &str, const std::string delim);
}
