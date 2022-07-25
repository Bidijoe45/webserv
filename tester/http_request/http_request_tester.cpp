#include "http_request_tester.hpp"

#include "./tests/http_request_tests.hpp"

namespace ws_tester
{
	HttpRequestTester::HttpRequestTester() {}

	void HttpRequestTester::test() {
		bool ret = false;

		ret = http_request_test1(1, true);
	}
} // namespace ws_tester