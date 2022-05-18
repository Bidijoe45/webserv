#include <iostream>

#include "../tests_utils.hpp"

#include "tests/settings_tests.hpp"
#include "settings_tester.hpp"

namespace ws_tester
{
	
	SettingsTester::SettingsTester() {}

	void SettingsTester::test() {
		bool ret = false;

		ret = settings_test_1(1, false);
		print_test_result(1, ret);

		ret = settings_test_2(2, false);
		print_test_result(2, ret);
	}

} // namespace ws_tester

