#include <iostream>

#include "settings_parser/settings_tester.hpp"
#include "settings_lexer/lexer_tester.hpp"
#include "http_parser/http_parser_tester.hpp"

void f()
{
	system("leaks webserv_test");
}

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

	if (arg == "all" || arg == "settings_lexer") {
		std::cout << "LEXER" << std::endl;
		ws_tester::LexerTester::test();
		std::cout << std::endl;
	}

	if (arg == "all" || arg == "settings_parser") {
		std::cout << "SETTINGS" << std::endl;
		ws_tester::SettingsTester::test();
		std::cout << std::endl;
	}

	if (arg == "all" || arg == "http_parser") {
		std::cout << "HTTP PARSER" << std::endl;
		ws_tester::HttpParserTester::test();
		std::cout << std::endl;
	}

	//atexit(&f);

	return 0;
}

