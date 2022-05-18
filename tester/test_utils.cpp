#include "tests_utils.hpp"

namespace ws_tester
{
	void print_test_result(size_t test_n, bool result) {
		std::cout << "Test " << test_n << ": " << (result ? "✅" : "❌") << std::endl;
	}
} // namespace ws
