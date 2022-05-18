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
    void advance();
	std::vector<Token> make_tokens();
	Token resolve_value_type(const std::string &value);
	Token make_value();

private:
	std::string str_;
	size_t str_len_;
	size_t pos_;
	char current_char_;
	std::map<std::string, TOKEN_TYPE> valid_value_types_;

};

}  // namespace ws
