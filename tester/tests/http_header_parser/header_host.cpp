#include <vector>
#include <string>
#include <iostream>
#include "../../../srcs/http/http_header_parser.hpp"
#include "../../../srcs/http/http_header_map.hpp"
#include "../../../srcs/http/headers/http_headers.hpp"

int main()
{
		std::vector<std::string> header_block;
		header_block.push_back("host: webserv");
		
		ws::HttpHeaderParser header_parser(header_block);
		ws::HttpHeaderMap headers = header_parser.parse_block();
	
		// TEST 0
		if (!header_parser.is_valid())
		{
			std::cout << "Failed test: 0" << std::endl;
			return 1;
		}
	
		// TEST 1
		if (headers.begin()->first != "host" || headers.begin()->second->value != "webserv")
		{
			std::cout << "Failed test: 1" << std::endl;
			return 1;
		}

		// TEST 2
		{
			ws::HttpHeaderHost *host_header = static_cast<ws::HttpHeaderHost *>(headers.begin()->second);
			if (host_header->host != "webserv" || host_header->port != -1)
			{
				std::cout << "Failed test: 2" << std::endl;
				return 1;
			}
		}

		// TEST 3
		{
			ws::HttpHeaderHost	host_header;
			host_header.set_value("webserv:3000");
			if (host_header.is_valid == false
				|| host_header.host != "webserv"
				|| host_header.port != 3000)
			{
				std::cout << "Failed test: 3" << std::endl;
				return 1;
			}
		}
		
		// TEST 4
		{
			ws::HttpHeaderHost	host_header;
			host_header.set_value("webserv:abcd");
			if (host_header.is_valid == true) 
			{
				std::cout << "Failed test: 4" << std::endl;
				return 1;
			}
		}

		// TEST 5
		{
			ws::HttpHeaderHost	host_header;
			host_header.set_value("127.0.0.1:80");
			if (host_header.is_valid == false
				|| host_header.host != "127.0.0.1"
				|| host_header.port != 80) 
			{
				std::cout << "Failed test: 5" << std::endl;
				return 1;
			}
		}

		// TEST 6
		{
			ws::HttpHeaderHost	host_header;
			host_header.set_value(":3000");
			if (host_header.is_valid == true) 
			{
				std::cout << "Failed test: 6" << std::endl;
				return 1;
			}
		}

		// TEST 7
		{
			ws::HttpHeaderHost	host_header;
			host_header.set_value("webserv: 3000");
			if (host_header.is_valid == true) 
			{
				std::cout << "Failed test: 7" << std::endl;
				return 1;
			}
		}

		// TEST 8
		{
			ws::HttpHeaderHost	host_header;
			host_header.set_value("WeBserV:3000");
			if (host_header.is_valid == false
				|| host_header.host != "webserv"
				|| host_header.port != 3000) 
			{
				std::cout << "Failed test: 8" << std::endl;
				return 1;
			}
		}

    return 0;
}
	
