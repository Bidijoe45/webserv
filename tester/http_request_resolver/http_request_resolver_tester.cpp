#include "../tests_utils.hpp"
#include "http_request_resolver_tester.hpp"
#include "tests/http_request_resolver_tests.hpp"

namespace ws_tester
{
	HttpRequestResolverTester::HttpRequestResolverTester() {}

	void HttpRequestResolverTester::test() {
		bool ret = false;

		ret = http_request_resolver_test1(1, true);
		print_test_result(1, ret);
	}
} // namespace ws_tester
