#include <typeinfo>
#include <iostream>

#include "http_parser_tests_utils.hpp"

#include "../../../srcs/server/data_buffer.hpp"
#include "../../../srcs/http/http_parser.hpp"
#include "../../../srcs/http/http_request.hpp"
#include "../../../srcs/http/http_uri.hpp"

namespace ws_tester
{

	/*static ws::HttpRequest prepare_request_case() {
		ws::HttpRequest request;

		request.method = ws::HTTP_METHOD_POST;
		request.uri = ws::HttpUri();
		request.http_version = "HTTP/1.1";
		request.headers.insert(std::make_pair("Host", "webserv.com"));
		request.headers.insert(std::make_pair("Content-Type", "application/x-www-form-urlencoded"));
		request.headers.insert(std::make_pair("Content-Length", "22"));
		request.body = "key1=value1&ke2=value2";

		return request;
	}*/

	static ws::DataBuffer prepare_data_buffer()
	{
		ws::DataBuffer buff;

		buff.append("POST / HTTP/1.1\r\n");
		buff.append("Host:  \twebserv.com\t\t \r\n");
		buff.append("Content-Type: application/x-www-form-urlencoded\r\n");
		buff.append("Content-Length: 22\r\n");
		buff.append("Accept: text/*, text/plain, text/plain;format=flowed, */*\r\n");
		buff.append("\r\n");
		buff.append("key1=value1&key2=value2");

		return buff;
	}

	bool http_parser_test3(size_t test_n, bool print)
	{
		ws::DataBuffer buff = prepare_data_buffer();
		ws::HttpParser parser(buff);

		//ws::HttpRequest prepared_request = prepare_request_case();
		ws::HttpRequest parsed_request = parser.parse();

		if (print) {
			//std::cout << "-- EXPECTED REQUEST --" << std::endl;
			//print_http_request(prepared_request);
			//std::cout << "-- PARSED REQUEST --" << std::endl;
			print_http_request(parsed_request);
		}

		//if (compare_requests(prepared_request, parsed_request)) {
		//	return true;
		//}

		return false;
	}

} // namespace ws_tester
