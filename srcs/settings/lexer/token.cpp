#include "token.hpp"

namespace ws
{
	Token::Token() {}
	Token::Token(TOKEN_TYPE type) : type(type) {}
	Token::Token(TOKEN_TYPE type, std::string value) : type(type), value(value) {}

	bool operator==(const Token &lhs, const Token &rhs) {
		if (lhs.type != rhs.type || lhs.value != rhs.value) {
			return false;
		}

		return true;
	}
} // namespace ws


