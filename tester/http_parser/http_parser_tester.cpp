#include "http_parser_tester.hpp"

#include "../tests_utils.hpp"
#include "./tests/http_parser_tests.hpp"

namespace ws_tester
{
	
	HttpParserTester::HttpParserTester() {}

	void HttpParserTester::test() {

		bool ret = false;

		ret = http_parser_test1(1, false);
		print_test_result(1, ret);

		ret = http_parser_test2(2, false);
		print_test_result(2, ret);

	}

} // namespace ws_tester