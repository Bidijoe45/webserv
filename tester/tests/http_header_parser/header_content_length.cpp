#include <vector>
#include <string>
#include <iostream>
#include "../../../srcs/http/http_header_parser.hpp"
#include "../../../srcs/http/http_header_map.hpp"
#include "../../../srcs/http/headers/http_headers.hpp"

int main()
{
	{
		std::vector<std::string> header_block;
		header_block.push_back("content-length: 10");
		
		ws::HttpHeaderParser header_parser(header_block);
		ws::HttpHeaderMap headers = header_parser.parse_block();
	
	// TEST 0
		if (!header_parser.is_valid())
		{
			std::cout << "Failed test: 0" << std::endl;
			return 1;
		}
	
	// TEST 1
		if (headers.begin()->first != "content-length" || headers.begin()->second->value != "10")
		{
			std::cout << "Failed test: 1" << std::endl;
			return 1;
		}

	// TEST 2
		ws::HttpHeaderContentLength *content_length = static_cast<ws::HttpHeaderContentLength*>(headers.begin()->second);
		if (content_length->content_length != 10)
		{
			std::cout << "Failed test: 2" << std::endl;
			return 1;
		}
	}

	// TEST 3
	{
		ws::HttpHeaderContentLength content_length;
		content_length.set_value("123");
		if (content_length.is_valid == false || content_length.content_length != 123)
		{
			std::cout << "Failed test: 3" << std::endl;
			return 1;
		}
	}
	
	// TEST 4
	{
		ws::HttpHeaderContentLength content_length;
		content_length.set_value("12a");
		if (content_length.is_valid == true)
		{
			std::cout << "Failed test: 4" << std::endl;
			return 1;
		}
	}

	// TEST 5
	{
		ws::HttpHeaderContentLength content_length;
		content_length.set_value(SIZE_MAX);
		if (content_length.is_valid == false || content_length.content_length != SIZE_MAX)
		{
			std::cout << "Failed test: 5" << std::endl;
			return 1;
		}
	}

    return 0;
}
	
