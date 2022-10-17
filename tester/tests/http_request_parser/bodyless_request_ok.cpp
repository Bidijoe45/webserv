#include <vector>
#include <string>
#include <iostream>

#include "../../../srcs/http/http_parser.hpp"
#include "../../../srcs/http/http_request.hpp"
#include "../../../srcs/server/data_buffer.hpp"
#include "../../../srcs/http/http_request_line_parser.hpp"
#include "../../../srcs/http/http_header_parser.hpp"

ws::HttpRequest generate_model_request()
{
	ws::HttpRequest http_request;

	ws::HttpRequestLineParser req_line_parser("GET / HTTP/1.1");
	http_request.request_line = req_line_parser.parse();

	std::vector<std::string> header_block;
	header_block.push_back("Host: webserv");
	header_block.push_back("Connection: close");
	ws::HttpHeaderParser header_parser(header_block);
	http_request.headers = header_parser.parse_block();

	http_request.body = "";

	http_request.error = ws::HttpRequest::NO_ERROR;

	return http_request;
}

int main()
{
	ws::HttpParser http_parser;
	ws::DataBuffer buff("GET / HTTP/1.1\r\nHost: webserv\r\nConnection: close\r\n\r\n");
	http_parser.parse(buff);
	ws::HttpRequest model_request = generate_model_request();
	ws::HttpRequest http_request = http_parser.get_request();

	// TEST 0
	if (http_parser.get_stage() != ws::HttpParser::COMPLETED)
	{
		std::cout << "Failed test 0" << std::endl;
		return 1;
	}

	// TEST 1
	if (http_request != model_request)
	{
		std::cout << "Failed test 1: ";
		if (http_request.request_line != model_request.request_line)
			std::cout << "|request line differs|" << std::endl;
		if (http_request.headers != model_request.headers)
			std::cout << "|headers differ|" << std::endl;
		if (http_request.body != model_request.body)
			std::cout << "|body differs|" << std::endl;
		if (http_request.error != model_request.error)
			std::cout << "|error differs|" << std::endl;
		return 1;
	}

	// TEST 2
	if (http_parser.must_close() != false)
	{
		std::cout << "Failed test 2" << std::endl;
		return 1;
	}

    return 0;
}
