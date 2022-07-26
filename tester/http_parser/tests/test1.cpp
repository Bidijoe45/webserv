
#include <typeinfo>
#include <iostream>

#include "http_parser_tests_utils.hpp"

#include "../../../srcs/server/data_buffer.hpp"
#include "../../../srcs/http/http_parser.hpp"
#include "../../../srcs/http/http_request.hpp"
#include "../../../srcs/http/http_uri.hpp"

namespace ws_tester
{

	static ws::HttpRequest prepare_request_case() {
		ws::HttpRequest request;

		request.method = ws::HTTP_METHOD_GET;
		request.uri = ws::HttpUri(); 
		request.headers.insert(std::make_pair("Host", "webserv.com"));

		return request;
	}

	static ws::DataBuffer prepare_data_buffer() {
		ws::DataBuffer buff;
		

		buff.append("GET / HTTP/1.1\n", strlen("GET / HTTP/1.1\n"));
		buff.append("Host: webserv.com\n", strlen("Host: webserv.com\n"));

		return buff;
	}

	bool http_parser_test1(size_t test_n, bool print) {
		
		ws::DataBuffer buff = prepare_data_buffer();
		ws::HttpParser parser(buff);

		ws::HttpRequest prepared_request = prepare_request_case();
		ws::HttpRequest parsed_request = parser.parse();

		if (print) {
			std::cout << "-- EXPECTED REQUEST --" << std::endl;
			print_http_request(prepared_request);
			std::cout << "-- PARSED REQUEST --" << std::endl;
			print_http_request(parsed_request);
		}
			

		if (compare_requests(prepared_request, parsed_request)) {
			return true;
		}

		return false;
	}

} // namespace ws_tester

