#include <string>
#include <iostream>
#include "../../../srcs/http/http_request_line_parser.hpp"
#include "../../../srcs/http/http_request_line.hpp"

int main()
{
	// TEST 0
	ws::HttpRequestLineParser parser0("GET           / HTTP/1.1");
	ws::HttpRequestLine req_line0 = parser0.parse();
	if (parser0.is_valid())
	{
		std::cout << "Failed test: 0" << std::endl;
		return 1;
	}

	// TEST 1
	ws::HttpRequestLineParser parser1("POST /\t HTTP/1.1");
	ws::HttpRequestLine req_line1 = parser1.parse();
	if (parser1.is_valid())
	{
		std::cout << "Failed test: 1" << std::endl;
		return 1;
	}

	// TEST 2
	ws::HttpRequestLineParser parser2(" DELETE / HTTP/1.1");
	ws::HttpRequestLine req_line2 = parser2.parse();
	if (parser2.is_valid())
	{
		std::cout << "Failed test: 2" << std::endl;
		return 1;
	}

	// TEST 3
	ws::HttpRequestLineParser parser3("\tCACA / HTTP/1.1");
	ws::HttpRequestLine req_line3 = parser3.parse();
	if (parser3.is_valid())
	{
		std::cout << "Failed test: 3" << std::endl;
		return 1;
	}

	// TEST 4
	ws::HttpRequestLineParser parser4("GET /  HTTP/1.1");
	ws::HttpRequestLine req_line4 = parser4.parse();
	if (parser4.is_valid())
	{
		std::cout << "Failed test: 4" << std::endl;
		return 1;
	}

	// TEST 5
	ws::HttpRequestLineParser parser5("GET / HTTP/1.1\n");
	ws::HttpRequestLine req_line5 = parser5.parse();
	if (parser5.is_valid())
	{
		std::cout << "Failed test: 5" << std::endl;
		return 1;
	}

	// TEST 6
	ws::HttpRequestLineParser parser6("GET \t/ HTTP/1.1");
	ws::HttpRequestLine req_line6 = parser6.parse();
	if (parser6.is_valid())
	{
		std::cout << "Failed test: 6" << std::endl;
		return 1;
	}

	// TEST 7
	ws::HttpRequestLineParser parser7("GET/ HTTP/1.1");
	ws::HttpRequestLine req_line7 = parser7.parse();
	if (parser7.is_valid())
	{
		std::cout << "Failed test: 7" << std::endl;
		return 1;
	}

	// TEST 8
	ws::HttpRequestLineParser parser8("GETHTTP11");
	ws::HttpRequestLine req_line8 = parser8.parse();
	if (parser8.is_valid())
	{
		std::cout << "Failed test: 8" << std::endl;
		return 1;
	}

	// TEST 9
	ws::HttpRequestLineParser parser9("GET /HTTP/1.1");
	ws::HttpRequestLine req_line9 = parser9.parse();
	if (parser9.is_valid())
	{
		std::cout << "Failed test: 9" << std::endl;
		return 1;
	}

	// TEST 10 
	ws::HttpRequestLineParser parser10("GET/HTTP/1.1");
	ws::HttpRequestLine req_line10 = parser10.parse();
	if (parser10.is_valid())
	{
		std::cout << "Failed test: 10" << std::endl;
		return 1;
	}

    return 0;
}
