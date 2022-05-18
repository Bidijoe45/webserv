#include "lexer_tests_utils.hpp"

#include <vector>

#include "../../../srcs/settings/lexer/settings_lexer.hpp"

void print_token_type(ws::TOKEN_TYPE token_type) {

	switch (token_type)
	{
		case ws::TT_SERVER:
			std::cout << "TT_SERVER";
			break;

		case ws::TT_LOCATION:
			std::cout << "TT_LOCATION";
			break;

		case ws::TT_LBRACKET:
			std::cout << "TT_LBRAKET";
			break;

		case ws::TT_RBRACKET:
			std::cout << "TT_RBRAKET";
			break;

		case ws::TT_VALUE:
			std::cout << "TT_VALUE";
			break;

		case ws::TT_SEMICOLON:
			std::cout << "TT_SEMICOLON";
			break;

		case ws::TT_LISTEN:
			std::cout << "TT_LISTEN";
			break;

		case ws::TT_SERVER_NAME:
			std::cout << "TT_SERVER_NAME";
			break;

		case ws::TT_ERROR_PAGE:
			std::cout << "TT_ERROR_PAGE";
			break;

		case ws::TT_CLIENT_MAX_BODY_SIZE:
			std::cout << "TT_CLIENT_MAX_BODY_SIZE";
			break;
		
		case ws::TT_ROOT:
			std::cout << "TT_ROOT";
			break;	
	
		case ws::TT_INDEX:
			std::cout << "TT_INDEX";
			break;	

		case ws::TT_ACCEPT:
			std::cout << "TT_ACCEPT";
			break;	

		case ws::TT_REWRITE:
			std::cout << "TT_REWRITE";
			break;	

		case ws::TT_AUTOINDEX:
			std::cout << "TT_AUTOINDEX";
			break;

		case ws::TT_CGI:
			std::cout << "TT_CGI";
			break;
		
		default:
			std::cout << "???";
			break;
	
	}

}

void print_lexer_tokens(const std::vector<ws::Token> &tokens) {
	std::vector<ws::Token>::const_iterator it = tokens.begin();
	std::vector<ws::Token>::const_iterator ite = tokens.end();

	std::cout << "---- TOKENS ----" << std::endl;

	for (; it != ite; it++) {
		const ws::Token &token = *it;

		std::cout << " --" << std::endl;
		std::cout << " type: ";
		print_token_type(token.type);
		std::cout << std::endl;
		std::cout << " value: " << token.value << std::endl;

		std::cout << " --" << std::endl;

	}

	std::cout << "----------------" << std::endl;
}