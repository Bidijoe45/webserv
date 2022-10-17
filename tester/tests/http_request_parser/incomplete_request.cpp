#include <vector>
#include <string>
#include <iostream>

#include "../../../srcs/http/http_parser.hpp"
#include "../../../srcs/http/http_request.hpp"
#include "../../../srcs/server/data_buffer.hpp"

int main()
{
	// TEST 0
	{
		ws::HttpParser http_parser;
		ws::DataBuffer buff("GET / HTT");
		http_parser.parse(buff);
	
		// TEST 0.0
		if (http_parser.get_stage() != ws::HttpParser::REQUEST_LINE || http_parser.get_request().error != ws::HttpRequest::NO_ERROR)
		{
			std::cout << "Failed test 0.0" << std::endl;
			return 1;
		}
	
		// TEST 0.1 
		if (http_parser.must_close() != false)
		{
			std::cout << "Failed test 0.1" << std::endl;
			return 1;
		}
	}

	// TEST 1
	{
		ws::HttpParser http_parser;
		ws::DataBuffer buff("GET / HTTP/1.1\r\nHo");
		http_parser.parse(buff);
	
		// TEST 1.0
		if (http_parser.get_stage() != ws::HttpParser::HEADERS_BLOCK || http_parser.get_request().error != ws::HttpRequest::NO_ERROR)
		{
			std::cout << "Failed test 1.0" << std::endl;
			return 1;
		}
	
		// TEST 1.1 
		if (http_parser.must_close() != false)
		{
			std::cout << "Failed test 1.1" << std::endl;
			return 1;
		}
	}

	// TEST 2
	{
		ws::HttpParser http_parser;
		ws::DataBuffer buff("GET / HTTP/1.1\r\nHost: webserv\r\nHeader2: another_header\r\n");
		http_parser.parse(buff);
	
		// TEST 2.0
		if (http_parser.get_stage() != ws::HttpParser::HEADERS_BLOCK || http_parser.get_request().error != ws::HttpRequest::NO_ERROR)
		{
			std::cout << "Failed test 2.0" << std::endl;
			return 1;
		}
	
		// TEST 2.1 
		if (http_parser.must_close() != false)
		{
			std::cout << "Failed test 2.1" << std::endl;
			return 1;
		}
	}
	
	// TEST 3
	{
		ws::HttpParser http_parser;
		ws::DataBuffer buff("GET / HTTP/1.1\r\nHost: webserv\r\nContent-length: 20\r\n\r\nincomplete body");
		http_parser.parse(buff);
	
		// TEST 3.0
		if (http_parser.get_stage() != ws::HttpParser::SIMPLE_BODY || http_parser.get_request().error != ws::HttpRequest::NO_ERROR)
		{
			std::cout << "Failed test 3.0" << std::endl;
			return 1;
		}
	
		// TEST 3.1 
		if (http_parser.must_close() != false)
		{
			std::cout << "Failed test 3.1" << std::endl;
			return 1;
		}
	}

	// TEST 4
	{
		ws::HttpParser http_parser;
		ws::DataBuffer buff("GET / HTTP/1.1\r\nHost: webserv\r\nTransfer-encoding: chunked\r\n\r\n");
		http_parser.parse(buff);
	
		// TEST 4.0
		if (http_parser.get_stage() != ws::HttpParser::CHUNKED_BODY || http_parser.get_request().error != ws::HttpRequest::NO_ERROR)
		{
			std::cout << "Failed test 4.0" << std::endl;
			return 1;
		}
	
		// TEST 4.1 
		if (http_parser.must_close() != false)
		{
			std::cout << "Failed test 4.1" << std::endl;
			return 1;
		}
	}

    return 0;
}
