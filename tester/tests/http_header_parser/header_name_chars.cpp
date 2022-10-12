#include <vector>
#include <string>
#include <iostream>
#include "../../../srcs/http/http_header_parser.hpp"
#include "../../../srcs/http/http_header_map.hpp"

int main()
{
	// TEST 0
	{
		std::vector<std::string> header_block;
		header_block.push_back(" : webserv");
		
		ws::HttpHeaderParser header_parser(header_block);
		ws::HttpHeaderMap headers = header_parser.parse_block();
	
		if (header_parser.is_valid())
		{
			std::cout << "Failed test: 0" << std::endl;
			return 1;
		}
	}
	
	// TEST 1
	{
		std::vector<std::string> header_block;
		header_block.push_back("\r");
		
		ws::HttpHeaderParser header_parser(header_block);
		ws::HttpHeaderMap headers = header_parser.parse_block();
	
		if (header_parser.is_valid())
		{
			std::cout << "Failed test: 1" << std::endl;
			return 1;
		}
	}

	// TEST 2
	{
		std::vector<std::string> header_block;
		header_block.push_back("\thost: webserv");
		
		ws::HttpHeaderParser header_parser(header_block);
		ws::HttpHeaderMap headers = header_parser.parse_block();
	
		if (header_parser.is_valid())
		{
			std::cout << "Failed test: 2" << std::endl;
			return 1;
		}
	}

	// TEST 3
	{
		std::vector<std::string> header_block;
		header_block.push_back("host : webserv");
		
		ws::HttpHeaderParser header_parser(header_block);
		ws::HttpHeaderMap headers = header_parser.parse_block();
	
		if (header_parser.is_valid())
		{
			std::cout << "Failed test: 3" << std::endl;
			return 1;
		}
	}

	// TEST 4
	{
		std::vector<std::string> header_block;
		header_block.push_back("host\r\n: webserv");
		
		ws::HttpHeaderParser header_parser(header_block);
		ws::HttpHeaderMap headers = header_parser.parse_block();
	
		if (header_parser.is_valid())
		{
			std::cout << "Failed test: 4" << std::endl;
			return 1;
		}
	}

    return 0;
}
	
