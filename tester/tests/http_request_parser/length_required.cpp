#include <vector>
#include <string>
#include <iostream>

#include "../../../srcs/http/http_parser.hpp"
#include "../../../srcs/http/http_request.hpp"
#include "../../../srcs/server/data_buffer.hpp"

int main()
{
	ws::HttpParser http_parser;
	ws::DataBuffer buff("GET / HTTP/1.1\r\nhost: webserv\r\nConnection: close\r\n\r\nesto es un body pero no hay content-length!");
	http_parser.parse(buff);
	ws::HttpRequest http_request = http_parser.get_request();

	// TEST 0
	if (http_request.error != ws::HttpRequest::LENGTH_REQUIRED)
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
	if (http_parser.must_close() != true)
	{
		std::cout << "Failed test 2" << std::endl;
		return 1;
	}

    return 0;
}
