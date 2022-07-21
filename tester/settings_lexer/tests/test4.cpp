#include <iostream>

#include "lexer_tests_utils.hpp"

#include "../../../srcs/settings/lexer/settings_lexer.hpp"
#include "../../../srcs/settings/lexer/token.hpp"


bool lexer_test4(size_t test_n, bool print) {

	ws::SettingsLexer lexer = ws::SettingsLexer("tester/settings_parser/test_files/not_real_file.conf");

	if (print) {
		std::cout << "---Correct output---" << std::endl;
		std::cout << "Output should be: File not found" << std::endl;
		std::cout << "---Output---" << std::endl;
		std::cout << lexer.get_error_msg() << std::endl;
	}

	return (lexer.file_is_valid() == false);
}
