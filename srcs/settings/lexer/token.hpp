#pragma once

#include <string>

namespace ws
{
	enum TOKEN_TYPE {
		TT_LBRACKET,
		TT_RBRACKET,
		TT_SEMICOLON,
		TT_SERVER,
		TT_LOCATION,
		TT_LISTEN,
		TT_SERVER_NAME,
		TT_ERROR_PAGE,
		TT_CLIENT_MAX_BODY_SIZE,
		TT_ROOT,
		TT_INDEX,
		TT_ACCEPT,
		TT_REWRITE,
		TT_AUTOINDEX,
		TT_CGI,
		TT_UPLOAD,
		TT_VALUE
	};

	struct Token {
		TOKEN_TYPE	type;
		std::string	value;

		Token();
		Token(TOKEN_TYPE type);
		Token(TOKEN_TYPE type, std::string value);
	};

	bool operator==(const Token &lhs, const Token &rhs);
} // namespace ws
