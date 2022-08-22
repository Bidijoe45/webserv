#include <typeinfo>
#include <iostream>

#include "http_uri_parser_tests_utils.hpp"
#include "../../../srcs/http/http_uri_parser.hpp"

namespace ws_tester
{

static ws::HttpUri prepare_uri()
{
	ws::HttpUri prepared_uri;

	return prepared_uri;
}

bool http_uri_parser_test1(size_t test_n, bool print)
{
	std::string uri = "/path/to/file.html?param1=value1&param2=value2";

	ws::HttpUriParser uri_parser(uri);
	ws::HttpUri parsed_uri = uri_parser.parse();
	ws::HttpUri prepared_uri = prepare_uri();

	if (print)
	{
		std::cout << "--- Prepared Uri ---" << std::endl;
		print_http_uri(prepared_uri);
		std::cout << "--- Parsed Uri ---" << std::endl;
		print_http_uri(parsed_uri);
	}

	return prepared_uri == parsed_uri;
}

}
