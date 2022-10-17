#pragma once

#include <string>
#include <vector>

namespace ws
{
	bool is_string_printable(const std::string &str, size_t len);
	bool is_string_digit(const std::string &str, size_t len);
	bool is_string_hex(const std::string &str, size_t len);
	std::string string_to_lower(const std::string &std, size_t len);
	bool is_token(const std::string &str);
	bool is_obstext(const unsigned char &c);
	std::vector<std::string> string_split(const std::string &str, const std::string &delim);
	std::string compress_slash(const std::string &str);
	std::string int_to_string(const int n);
	std::string ul_to_string(const unsigned long int n);
	std::string string_trim_left(const std::string &str, const std::string chars);
	std::string string_trim_right(const std::string &str, const std::string chars);
	std::string string_trim(const std::string &str, const std::string chars);
	bool str_is_numeric(const std::string &str);
	size_t compare_split_paths(const std::vector<std::string> &path1, const std::vector<std::string> &path2);
}
