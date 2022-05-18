#pragma once

#include <string>

#include "../settings.hpp"
#include "../lexer/token.hpp"

namespace ws {

class SettingsParser {
	public:
		SettingsParser(std::string settings_file);
		Settings parse();

	private:
		bool check_tokens_validity(const std::vector<Token> &tokens);
		Settings generate_settings(const std::vector<Token> &tokens);
		void check_semicolon();
		void check_left_bracket();
		void check_right_bracket();
		std::string resolve_root_element();
		std::string resolve_index_element();
		bool resolve_autoindex_element();
		std::vector<HTTP_METHOD> resolve_accept_element();
		Rewrite resolve_rewrite_element();
		size_t resolve_listen_element();
		std::string resolve_server_name();
		ServerSettings resolve_server_block();
		size_t resolve_client_max_body_size();
		ErrorPage resolve_error_page();
		Location resolve_location_block();
		void advance();
		void back();

		std::string settings_file_;
		std::vector<Token> tokens_;
		size_t n_tokens_;
		int pos_;
		Token current_token_;
		Settings settings_;
};

}