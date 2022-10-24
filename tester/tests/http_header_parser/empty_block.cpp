#include <vector>
#include <string>
#include <iostream>
#include "../../../srcs/http/http_header_parser.hpp"
#include "../../../srcs/http/http_header_map.hpp"

int main()
{
	std::vector<std::string> header_block;

	ws::HttpHeaderParser header_parser(header_block);
	ws::HttpHeaderMap headers = header_parser.parse_block();

	if (!header_parser.is_valid())
		return 1;
	if (headers.size() != 0)
		return 1;
	if (headers != ws::HttpHeaderMap())
		return 1;

    return 0;
}
	
