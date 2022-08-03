#include <string>
#include <vector>
#include <cctype>
#include <string>
#include <iostream>

#include "string_utils.hpp"

namespace ws
{
	bool is_string_printable(const std::string &str, size_t len)
	{
		for (int i = 0; (str[i] && i < len); i++)
		{
			std::cout << "is_print: " << int(str[i]) << std::endl;
			if (!std::isprint(str[i]))
				return false;
		}
		return true;
	}

	std::vector<std::string> string_split(const std::string &str, const std::string &delim) {
    	std::vector<std::string> tokens;
    	size_t start;
    	size_t end = 0;

    	while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
        	end = str.find(delim, start);
        	tokens.push_back(str.substr(start, end - start));
    	}

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
}
