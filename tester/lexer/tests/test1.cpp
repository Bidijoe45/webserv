#include <iostream>

#include "lexer_tests_utils.hpp"

#include "../../../srcs/settings/lexer/settings_lexer.hpp"
#include "../../../srcs/settings/lexer/token.hpp"

static std::vector<ws::Token> prepare_tokens() {
	std::vector<ws::Token> tokens;
	
	//server
	tokens.push_back(ws::Token(ws::TT_SERVER));

	//{
	tokens.push_back(ws::Token(ws::TT_LBRACKET));

		//---
		//listen
		tokens.push_back(ws::Token(ws::TT_LISTEN));

		//80
		tokens.push_back(ws::Token(ws::TT_VALUE, "80"));
		
		//;
		tokens.push_back(ws::Token(ws::TT_SEMICOLON));
		//---

		//---
		//server_name
		tokens.push_back(ws::Token(ws::TT_SERVER_NAME));

		//test_server_1
		tokens.push_back(ws::Token(ws::TT_VALUE, "test_server_1"));
		
		//;
		tokens.push_back(ws::Token(ws::TT_SEMICOLON));
		//---

		//---
		//error_page
		tokens.push_back(ws::Token(ws::TT_ERROR_PAGE));

		//404
		tokens.push_back(ws::Token(ws::TT_VALUE, "404"));

		// /404.html
		tokens.push_back(ws::Token(ws::TT_VALUE, "/404.html"));
		
		//;
		tokens.push_back(ws::Token(ws::TT_SEMICOLON));
		//---

		//---
		//client_max_body_size
		tokens.push_back(ws::Token(ws::TT_CLIENT_MAX_BODY_SIZE));

		//1024
		tokens.push_back(ws::Token(ws::TT_VALUE, "1024"));
		
		//;
		tokens.push_back(ws::Token(ws::TT_SEMICOLON));
		//---

	//}
	tokens.push_back(ws::Token(ws::TT_RBRACKET));


	return tokens;
}

bool lexer_test1(size_t test_n, bool print) {
	std::vector<ws::Token> prepared_tokens = prepare_tokens();

	ws::SettingsLexer lexer = ws::SettingsLexer("tester/settings/test_files/conf_file1.conf");
	std::vector<ws::Token> tokens = lexer.make_tokens();

	if (print) {
		std::cout << "---Correct output---" << std::endl;
		print_lexer_tokens(prepared_tokens);

		std::cout << "---Output---" << std::endl;
		print_lexer_tokens(tokens);
	}

	return prepared_tokens == tokens;
}
