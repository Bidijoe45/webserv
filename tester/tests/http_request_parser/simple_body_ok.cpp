#include <vector>
#include <string>
#include <iostream>

#include "../../../srcs/http/http_request.hpp"
#include "../../../srcs/http/http_request_line_parser.hpp"
#include "../../../srcs/http/http_header_parser.hpp"
#include "../../../srcs/server/connection.hpp"
#include "../../../srcs/server/server.hpp"
#include "../../../srcs/settings/parser/settings_parser.hpp"

ws::HttpRequest generate_model_request()
{
	ws::HttpRequest http_request;

	ws::HttpRequestLineParser req_line_parser("GET / HTTP/1.1");
	http_request.request_line = req_line_parser.parse();

	std::vector<std::string> header_block;
	header_block.push_back("Host: webserv");
	header_block.push_back("Content-length: 5");
	ws::HttpHeaderParser header_parser(header_block);
	http_request.headers = header_parser.parse_block();

	http_request.body = "Hello";

	http_request.error = ws::HttpRequest::NO_ERROR;

	return http_request;
}

int main()
{
	ws::Connection connection;
	connection.port = 3000;

	ws::Server server;

	ws::SettingsParser settings_parser("./tests/http_request_parser/server.conf");
	server.set_settings(settings_parser.parse());
	if (!settings_parser.is_valid())
	{
		std::cout << "server config file invalid: " << settings_parser.get_error_msg() << std::endl;
		return 1;
	}

	connection.buff.append("GET / HTTP/1.1\r\nHost: webserv\r\nContent-length: 5\r\n\r\nHello");
	server.parse_request(connection);

	ws::HttpRequest model_request = generate_model_request();
	ws::HttpRequest http_request = connection.http_parser.get_request();

	// TEST 0
	if (connection.http_parser.get_stage() != ws::HttpParser::COMPLETED)
	{
		std::cout << "Failed test 0" << std::endl;
		return 1;
	}

	// TEST 1
	if (http_request != model_request)
	{
		std::cout << "Failed test 1: ";
		if (http_request.request_line != model_request.request_line)
			std::cout << "|request line differs|";
		if (http_request.headers != model_request.headers)
			std::cout << "|headers differ|";
		if (http_request.body != model_request.body)
			std::cout << "|body differs|";
		if (http_request.error != model_request.error)
			std::cout << "|error differs|";
		std::cout << std::endl;
		return 1;
	}

	// TEST 2
	if (connection.http_parser.must_close != false)
	{
		std::cout << "Failed test 2" << std::endl;
		return 1;
	}

    return 0;
}
