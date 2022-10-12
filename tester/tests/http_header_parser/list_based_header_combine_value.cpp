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
		header_block.push_back("transfer-encoding: header_value");
		header_block.push_back("transfer-encoding: another_header_value, third_header_value");
		
		ws::HttpHeaderParser header_parser(header_block);
		ws::HttpHeaderMap headers = header_parser.parse_block();
	
		if (!header_parser.is_valid() || headers.size() != 1 || headers.begin()->first != "transfer-encoding"
			|| headers.begin()->second->value != "header_value, another_header_value, third_header_value")
		{
			std::cout << "Failed test: 0" << std::endl;
			return 1;
		}
	}
	
	// TEST 1
	{
		std::vector<std::string> header_block;
		header_block.push_back("transfer-encodinG: hola");
		header_block.push_back("transfEr-encoding:");
		header_block.push_back("transfer-encodiNg: ");
		header_block.push_back("transfer-encOding: que tal   , ,adios");
		
		ws::HttpHeaderParser header_parser(header_block);
		ws::HttpHeaderMap headers = header_parser.parse_block();
	
		if (!header_parser.is_valid() || headers.size() != 1 || headers.begin()->first != "transfer-encoding"
			|| headers.begin()->second->value != "hola, , , que tal   , ,adios") // the parser of the list-based header will remove the empty elements
		{
			std::cout << "Failed test: 1" << std::endl;
			return 1;
		}
	}

    return 0;
}
	
