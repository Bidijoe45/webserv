#include <iostream>
#include <vector>

#include "../tests_utils.hpp"

#include "tests/lexer_tests.hpp"
#include "lexer_tester.hpp"

namespace ws_tester
{
	
	LexerTester::LexerTester() {}

	void LexerTester::test() {

		bool ret = false;

		ret = lexer_test1(1, false);
		print_test_result(1, ret);

		ret = lexer_test2(2, false);
		print_test_result(2, ret);

		ret = lexer_test3(3, false);
		print_test_result(3, ret);

	}

} // namespace ws_tester

