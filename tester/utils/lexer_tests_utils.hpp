#pragma once

#include <vector>

#include "../../srcs/settings/lexer/settings_lexer.hpp"

void print_token_type(ws::TOKEN_TYPE token_type);
void print_lexer_tokens(const std::vector<ws::Token> &tokens);
