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

	// TEST 0
	{
		connection.buff.append("GET / HTT");
		server.parse_request(connection);

		// TEST 0.0
		if (connection.http_parser.get_stage() != ws::HttpParser::REQUEST_LINE || connection.http_parser.get_request().error != ws::HttpRequest::NO_ERROR)
		{
			std::cout << "Failed test 0.0" << std::endl;
			return 1;
		}
	
		// TEST 0.1 
		if (connection.http_parser.must_close != false)
		{
			std::cout << "Failed test 0.1" << std::endl;
			return 1;
		}
	}

	// TEST 1
	{
		connection.buff.clear();
		connection.http_parser.reset();
		connection.buff.append("GET / HTTP/1.1\r\nHo");
		server.parse_request(connection);

		// TEST 1.0
		if (connection.http_parser.get_stage() != ws::HttpParser::HEADERS_BLOCK || connection.http_parser.get_request().error != ws::HttpRequest::NO_ERROR)
		{
			std::cout << "Failed test 1.0" << std::endl;
			return 1;
		}
	
		// TEST 1.1 
		if (connection.http_parser.must_close != false)
		{
			std::cout << "Failed test 1.1" << std::endl;
			return 1;
		}
	}

	// TEST 2
	{
		connection.buff.clear();
		connection.http_parser.reset();
		connection.buff.append("GET / HTTP/1.1\r\nHost: webserv\r\nHeader2: another_header\r\n");
		server.parse_request(connection);
	
		// TEST 2.0
		if (connection.http_parser.get_stage() != ws::HttpParser::HEADERS_BLOCK || connection.http_parser.get_request().error != ws::HttpRequest::NO_ERROR)
		{
			std::cout << "Failed test 2.0" << std::endl;
			return 1;
		}
	
		// TEST 2.1 
		if (connection.http_parser.must_close != false)
		{
			std::cout << "Failed test 2.1" << std::endl;
			return 1;
		}
	}
	
	// TEST 3
	{
		connection.buff.clear();
		connection.http_parser.reset();
		connection.buff.append("GET / HTTP/1.1\r\nHost: webserv\r\nContent-length: 20\r\n\r\nincomplete body");
		server.parse_request(connection);
	
		// TEST 3.0
		if (connection.http_parser.get_stage() != ws::HttpParser::SIMPLE_BODY || connection.http_parser.get_request().error != ws::HttpRequest::NO_ERROR)
		{
			std::cout << "Failed test 3.0" << std::endl;
			return 1;
		}
	
		// TEST 3.1 
		if (connection.http_parser.must_close != false)
		{
			std::cout << "Failed test 3.1" << std::endl;
			return 1;
		}
	}

	// TEST 4
	{
		connection.buff.clear();
		connection.http_parser.reset();
		connection.buff.append("GET / HTTP/1.1\r\nHost: webserv\r\nTransfer-encoding: chunked\r\n\r\n");
		server.parse_request(connection);
	
		// TEST 4.0
		if (connection.http_parser.get_stage() != ws::HttpParser::CHUNKED_BODY || connection.http_parser.get_request().error != ws::HttpRequest::NO_ERROR)
		{
			std::cout << "Failed test 4.0" << std::endl;
			return 1;
		}
	
		// TEST 4.1 
		if (connection.http_parser.must_close != false)
		{
			std::cout << "Failed test 4.1" << std::endl;
			return 1;
		}
	}

    return 0;
}
