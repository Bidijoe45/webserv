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
}
