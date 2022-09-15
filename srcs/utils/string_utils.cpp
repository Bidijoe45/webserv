#include <string>
#include <vector>
#include <cctype>
#include <string>
#include <iostream>
#include <sstream>

#include "string_utils.hpp"

namespace ws
{
	bool is_string_printable(const std::string &str, size_t len)
	{
		for (int i = 0; (str[i] && i < len); i++)
		{
			if (!std::isprint(str[i]))
				return false;
		}
		return true;
	}

	std::vector<std::string> string_split(const std::string &str, const std::string &delim) {
    	std::vector<std::string> tokens;
    	size_t found = 0;
    	size_t start = 0;
    	size_t from = 0;
		
		while ((found = str.find(delim, from)) != std::string::npos)
		{
			if (start >= found)
			{
				start = found + delim.size();
				from = found + delim.size();
				continue ;
			}
			tokens.push_back(str.substr(start, found - start));
			start = found + delim.size();
			from = found + delim.size();
		}

		if (start < str.size())
			tokens.push_back(str.substr(start, str.size() - start));

    	return tokens;
    }
	std::string string_to_lower(const std::string &str, size_t len)
	{
		std::string	new_str;

		new_str.resize(str.size());
		for (int i = 0; (str[i] && i < len); i++)
			new_str[i] = tolower(str[i]);
		return new_str;
	}

	bool is_token(const std::string &str)
	{
		std::string allowed_chars = "!#$%&'*+-.^_`|~";

		for (size_t i = 0; str[i]; i++)
		{
			if (!isalpha(str[i]) && !isdigit(str[i]) && allowed_chars.find(str[i]) == std::string::npos)
				return false;
		}
		return true;
	}

	bool is_obstext(const unsigned char &c)
	{
		if (c >= 128)
			return true;
		return false;
	}

	std::string compress_slash(const std::string &str)
	{
		std::string new_str;

		for (size_t i=0; i < str.size(); i++)
		{
			if (str[i] == '/' && str[i + 1] == '/' && i < str.size())
				continue ;
			new_str.push_back(str[i]);
		}

		return new_str;
	}

	std::string int_to_string(const int n)
	{
		std::ostringstream s;
		s << n;
		std::string str = s.str();

		return str;
	}

	std::string string_trim_left(const std::string &str, const std::string delim)
	{
		size_t left_delim = str.find_first_not_of(delim);

		if (left_delim == std::string::npos)
			return str;

		return str.substr(left_delim);
	}

	std::string string_trim_right(const std::string &str, const std::string delim)
	{
		size_t right_delim = str.find_last_not_of(delim);

		if (right_delim == std::string::npos)
			return str;

		return str.substr(0, right_delim + 1);
	}

	std::string string_trim(const std::string &str, const std::string delim)
	{
		std::string left_trim = string_trim_left(str, delim);
		std::string right_trim = string_trim_right(left_trim, delim);
		return right_trim;
	}
}
