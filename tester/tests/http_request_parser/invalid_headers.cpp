#include <vector>
#include <string>
#include <iostream>

#include "../../../srcs/http/http_parser.hpp"
#include "../../../srcs/http/http_request.hpp"
#include "../../../srcs/server/data_buffer.hpp"

int main()
{
	{
		ws::HttpParser http_parser;
		ws::DataBuffer buff("GET / HTTP/1.1\r\nHost :webserv\r\nConnection: close\r\n\r\n");
		http_parser.parse(buff);
		ws::HttpRequest http_request = http_parser.get_request();
	
		// TEST 0
		if (http_request.error != ws::HttpRequest::BAD_REQUEST)
		{
			std::cout << "Failed test 0" << std::endl;
			return 1;
		}
	
		// TEST 1
		if (http_parser.get_stage() != ws::HttpParser::COMPLETED)
		{
			std::cout << "Failed test 1" << std::endl;
			return 1;
		}
	
		// TEST 2
		if (http_parser.must_close() != false)
		{
			std::cout << "Failed test 2" << std::endl;
			return 1;
		}
	}

	{
		ws::HttpParser http_parser;
		ws::DataBuffer buff("GET / HTTP/1.1\r\nheader: not host\r\nConnection: close\r\n\r\n");
		http_parser.parse(buff);
		ws::HttpRequest http_request = http_parser.get_request();
	
		// TEST 3
		if (http_request.error != ws::HttpRequest::BAD_REQUEST)
		{
			std::cout << "Failed test 3" << std::endl;
			return 1;
		}
	
		// TEST 4
		if (http_parser.get_stage() != ws::HttpParser::COMPLETED)
		{
			std::cout << "Failed test 4" << std::endl;
			return 1;
		}
	
		// TEST 5
		if (http_parser.must_close() != false)
		{
			std::cout << "Failed test 5" << std::endl;
			return 1;
		}
	}

    return 0;
}
