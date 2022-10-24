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
		header_block.push_back("connection: close");
		
		ws::HttpHeaderParser header_parser(header_block);
		ws::HttpHeaderMap headers = header_parser.parse_block();
	
	// TEST 0
		if (!header_parser.is_valid())
		{
			std::cout << "Failed test: 0" << std::endl;
			return 1;
		}
	
	// TEST 1
		if (headers.begin()->first != "connection" || headers.begin()->second->value != "close")
		{
			std::cout << "Failed test: 1" << std::endl;
			return 1;
		}

	// TEST 2
		ws::HttpHeaderConnection *connection = static_cast<ws::HttpHeaderConnection*>(headers.begin()->second);
		if (connection->options.size() != 1 || connection->options[0] != "close")
		{
			std::cout << "Failed test: 2" << std::endl;
			return 1;
		}
	}

	// TEST 3
	{
		ws::HttpHeaderConnection connection;
		connection.set_value("CloSE");
		if (connection.is_valid == false
			|| connection.options.size() != 1
			|| connection.options[0] != "close")
		{
			std::cout << "Failed test: 3" << std::endl;
			return 1;
		}
	}
	
	// TEST 4
	{
		ws::HttpHeaderConnection connection;
		connection.set_value("pipipi, papapa, pupupu");
		if (connection.is_valid == false
			|| connection.options.size() != 3
			|| connection.options[0] != "pipipi"
			|| connection.options[1] != "papapa"
			|| connection.options[2] != "pupupu")
		{
			std::cout << "Failed test: 4" << std::endl;
			return 1;
		}
	}

	// TEST 5
	{
		ws::HttpHeaderConnection connection;
		connection.set_value(",   pipipi, papapa  \t, pupupu , ,,");
		if (connection.is_valid == false
			|| connection.options.size() != 3
			|| connection.options[0] != "pipipi"
			|| connection.options[1] != "papapa"
			|| connection.options[2] != "pupupu")
		{
			std::cout << "Failed test: 5" << std::endl;
			return 1;
		}
	}

	// TEST 6
	{
		ws::HttpHeaderConnection connection;
		connection.set_value("  , ,,,  ,   ,");
		if (connection.is_valid == false || connection.options.size() > 0) 
		{
			std::cout << "Failed test: 6" << std::endl;
			return 1;
		}
	}
    return 0;
}
	
