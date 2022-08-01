#include <string>
#include <vector>

#include "string_utils.hpp"

namespace ws
{
	bool is_string_printable(std::string str, size_t len)
	{
		for (int i = 0; i < len; i++)
		{
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
}
