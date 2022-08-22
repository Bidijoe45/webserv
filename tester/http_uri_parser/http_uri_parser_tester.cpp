#include "http_uri_parser_tester.hpp"
#include "tests/http_uri_parser_tests.hpp"

namespace ws_tester
{
	HttpUriParserTester::HttpUriParserTester() {}

	void HttpUriParserTester::test() {
		bool ret = false;

		ret = http_uri_parser_test1(1, true);
	}
} // namespace ws_tester
