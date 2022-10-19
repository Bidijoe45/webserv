#include <vector>
#include <string>
#include <iostream>

#include "../../../srcs/http/http_request.hpp"
#include "../../../srcs/http/http_request_line_parser.hpp"
#include "../../../srcs/http/http_header_parser.hpp"
#include "../../../srcs/server/connection.hpp"
#include "../../../srcs/server/server.hpp"
#include "../../../srcs/settings/parser/settings_parser.hpp"

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

	{
		connection.buff.append("GET /   HTTP/1.1\r\nHost: webserv\r\nConnection: close\r\n\r\n");
		server.parse_request(connection);

		ws::HttpRequest http_request = connection.http_parser.get_request();
	
		// TEST 0
		if (http_request.error != ws::HttpRequest::BAD_REQUEST)
		{
			std::cout << "Failed test 0" << std::endl;
			return 1;
		}
	
		// TEST 1
		if (connection.http_parser.get_stage() != ws::HttpParser::COMPLETED)
		{
			std::cout << "Failed test 1" << std::endl;
			return 1;
		}
	
		// TEST 2
		if (connection.http_parser.must_close != false)
		{
			std::cout << "Failed test 2" << std::endl;
			return 1;
		}
	}

	{
		connection.buff.clear();
		connection.http_parser.reset();
		connection.buff.append("GET / HTTP/21.1\r\nHost: webserv\r\nConnection: close\r\n\r\n");
		server.parse_request(connection);

		ws::HttpRequest http_request = connection.http_parser.get_request();
	
		// TEST 3
		if (http_request.error != ws::HttpRequest::INVALID_VERSION)
		{
			std::cout << "Failed test 3" << std::endl;
			return 1;
		}
	
		// TEST 4
		if (connection.http_parser.get_stage() != ws::HttpParser::COMPLETED)
		{
			std::cout << "Failed test 4" << std::endl;
			return 1;
		}
	
		// TEST 5
		if (connection.http_parser.must_close != false)
		{
			std::cout << "Failed test 5" << std::endl;
			return 1;
		}
	}

    return 0;
}
