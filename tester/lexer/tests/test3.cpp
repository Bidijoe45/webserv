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

		//--
		//error_page
		tokens.push_back(ws::Token(ws::TT_ERROR_PAGE));

		//404
		tokens.push_back(ws::Token(ws::TT_VALUE, "404"));

		// /404.html
		tokens.push_back(ws::Token(ws::TT_VALUE, "/404.html"));

		//;
		tokens.push_back(ws::Token(ws::TT_SEMICOLON));
		//--

		//--
		//client_max_body_size
		tokens.push_back(ws::Token(ws::TT_CLIENT_MAX_BODY_SIZE));

		tokens.push_back(ws::Token(ws::TT_VALUE, "1024"));

		//;
		tokens.push_back(ws::Token(ws::TT_SEMICOLON));
		//--

		//---
		//location
		tokens.push_back(ws::Token(ws::TT_LOCATION));

		// /
		tokens.push_back(ws::Token(ws::TT_VALUE, "/"));
		
		// {
		tokens.push_back(ws::Token(ws::TT_LBRACKET));
		
			//root
			tokens.push_back(ws::Token(ws::TT_ROOT));

			// /www/html/mi_web
			tokens.push_back(ws::Token(ws::TT_VALUE, "/www/html/mi_web"));

			//;
			tokens.push_back(ws::Token(ws::TT_SEMICOLON));
			//--

			//index
			tokens.push_back(ws::Token(ws::TT_INDEX));

			// index.html
			tokens.push_back(ws::Token(ws::TT_VALUE, "index.html"));

			//;
			tokens.push_back(ws::Token(ws::TT_SEMICOLON));
			//--

			//--
			//accept
			tokens.push_back(ws::Token(ws::TT_ACCEPT));

			//GET
			tokens.push_back(ws::Token(ws::TT_VALUE, "GET"));

			//POST
			tokens.push_back(ws::Token(ws::TT_VALUE, "POST"));

			//DELETE
			tokens.push_back(ws::Token(ws::TT_VALUE, "DELETE"));

			//;
			tokens.push_back(ws::Token(ws::TT_SEMICOLON));
			//--

			//--
			//rewrite
			tokens.push_back(ws::Token(ws::TT_REWRITE));

			// /search
			tokens.push_back(ws::Token(ws::TT_VALUE, "/search"));

			// www.gooogle.com
			tokens.push_back(ws::Token(ws::TT_VALUE, "www.google.com"));

			//;
			tokens.push_back(ws::Token(ws::TT_SEMICOLON));
			//--

			//--
			//autoindex
			tokens.push_back(ws::Token(ws::TT_AUTOINDEX));

			//off
			tokens.push_back(ws::Token(ws::TT_VALUE, "on"));

			//;
			tokens.push_back(ws::Token(ws::TT_SEMICOLON));
			//--


		// }
		tokens.push_back(ws::Token(ws::TT_RBRACKET));

		//---
		//location
		tokens.push_back(ws::Token(ws::TT_LOCATION));

		// /
		tokens.push_back(ws::Token(ws::TT_VALUE, "/login"));
		
		// {
		tokens.push_back(ws::Token(ws::TT_LBRACKET));
		
			//root
			tokens.push_back(ws::Token(ws::TT_ROOT));

			// /www/html/mi_web
			tokens.push_back(ws::Token(ws::TT_VALUE, "/www/html/mi_web/login.html"));

			//;
			tokens.push_back(ws::Token(ws::TT_SEMICOLON));
			//--

			//--
			//accept
			tokens.push_back(ws::Token(ws::TT_ACCEPT));

			//POST
			tokens.push_back(ws::Token(ws::TT_VALUE, "POST"));

			//;
			tokens.push_back(ws::Token(ws::TT_SEMICOLON));
			//--

		// }
		tokens.push_back(ws::Token(ws::TT_RBRACKET));


		//---
		//location
		tokens.push_back(ws::Token(ws::TT_LOCATION));

		// /about
		tokens.push_back(ws::Token(ws::TT_VALUE, "/about"));
		
		// {
		tokens.push_back(ws::Token(ws::TT_LBRACKET));
		
			//root
			tokens.push_back(ws::Token(ws::TT_ROOT));

			// /www/html/mi_web
			tokens.push_back(ws::Token(ws::TT_VALUE, "/www/html/mi_web/pages/about.html"));

			//;
			tokens.push_back(ws::Token(ws::TT_SEMICOLON));
			//--

			//--
			//accept
			tokens.push_back(ws::Token(ws::TT_ACCEPT));

			//GET
			tokens.push_back(ws::Token(ws::TT_VALUE, "GET"));

			//DELETE
			tokens.push_back(ws::Token(ws::TT_VALUE, "DELETE"));

			//;
			tokens.push_back(ws::Token(ws::TT_SEMICOLON));
			//--

			//--
			//rewrite
			tokens.push_back(ws::Token(ws::TT_REWRITE));

			// /info
			tokens.push_back(ws::Token(ws::TT_VALUE, "/info"));

			// www.gooogle.com
			tokens.push_back(ws::Token(ws::TT_VALUE, "/about"));

			// www.gooogle.com
			tokens.push_back(ws::Token(ws::TT_VALUE, "permanent"));

			//;
			tokens.push_back(ws::Token(ws::TT_SEMICOLON));
			//--

			//--
			//autoindex
			tokens.push_back(ws::Token(ws::TT_AUTOINDEX));

			//off
			tokens.push_back(ws::Token(ws::TT_VALUE, "off"));

			//;
			tokens.push_back(ws::Token(ws::TT_SEMICOLON));
			//--


		// }
		tokens.push_back(ws::Token(ws::TT_RBRACKET));


	//}
	tokens.push_back(ws::Token(ws::TT_RBRACKET));


	return tokens;
}

bool lexer_test3(size_t test_n, bool print) {
	std::vector<ws::Token> prepared_tokens = prepare_tokens();

	ws::SettingsLexer lexer = ws::SettingsLexer("tester/settings/test_files/conf_file3.conf");
	std::vector<ws::Token> tokens = lexer.make_tokens();

	if (print) {
		std::cout << "---Correct output---" << std::endl;
		print_lexer_tokens(prepared_tokens);

		std::cout << "---Output---" << std::endl;
		print_lexer_tokens(tokens);
	}

	return prepared_tokens == tokens;
}
