#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "token.hpp"

namespace ws {

class SettingsLexer {

public:
    SettingsLexer(std::string file_name);
	std::vector<Token> make_tokens();

private:
	std::map<std::string, TOKEN_TYPE> valid_value_types_;
	void advance();
	Token resolve_value_type(const std::string &value);
	Token make_value();

	std::string str_;
	size_t str_len_;
	size_t pos_;
	char current_char_;

};

}  // namespace ws
