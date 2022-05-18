#include <iostream>

#include "settings/settings_tester.hpp"
#include "lexer/lexer_tester.hpp"

int main(int argc, char **argv) {

	std::string arg;

	if (argc == 1) {
		arg = "all";
	} else if (argc == 2) {
		arg = std::string(argv[1]);
	} else {
		std::cout << "Tester: invalid arguments" << std::endl;
		return 1;
	}

	if (arg == "all" || arg == "lexer") {
		std::cout << "LEXER" << std::endl;
		ws_tester::LexerTester::test();
	}
	std::cout << std::endl;

	if (arg == "all" || arg == "settings") {
		std::cout << "SETTINGS" << std::endl;
		ws_tester::SettingsTester::test();
	}
	std::cout << std::endl;

	return 0;
}

