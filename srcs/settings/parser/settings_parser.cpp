#include <string>

#include "settings_parser.hpp"
#include "../settings.hpp"
#include "../lexer/settings_lexer.hpp"
#include "../lexer/token.hpp"

namespace ws {

	SettingsParser::SettingsParser(std::string settings_file) : settings_file_(settings_file) {
		SettingsLexer lexer = SettingsLexer(this->settings_file_);
		this->tokens_ = lexer.make_tokens();
		this->n_tokens_ = this->tokens_.size();
		this->pos_ = 0;
		this->current_token_ = this->tokens_[this->pos_];
	}

	void SettingsParser::advance() {
		if (this->pos_ == this->n_tokens_ - 1) {
			this->pos_ = -1;
			return;
		}
		
		this->pos_++;
		this->current_token_ = this->tokens_[this->pos_];
	}

	void SettingsParser::back() {
		this->pos_--;
		this->current_token_ = this->tokens_[this->pos_];
	}

	void SettingsParser::check_semicolon() {
	
		if (this->current_token_.type != TT_SEMICOLON){
			std::cout << "Settings Parser: Missing semicolon" << std::endl;
			exit(1);
		}

		this->advance();
	}

	void SettingsParser::check_left_bracket() {
		
		if (this->current_token_.type != TT_LBRACKET){
			std::cout << "Settings Parser: Missing left bracket" << std::endl;
			exit(1);
		}

		this->advance();
	}

	void SettingsParser::check_right_bracket() {
		if (this->current_token_.type != TT_RBRACKET){
			std::cout << "Settings Parser: Missing right bracket" << std::endl;
			exit(1);
		}

		this->advance();
	}

	size_t SettingsParser::resolve_listen_element() {
		size_t port_n;

		if (this->current_token_.type != TT_VALUE
			&& this->current_token_.value.find_first_not_of("0123456789") != std::string::npos)
		{
			std::cout << "Settings Parser: Invalid listen argument" << std::endl;
			exit(1);
		}

		port_n = std::atoll(this->current_token_.value.c_str());
		this->advance();

		return port_n;
	}

	std::string SettingsParser::resolve_server_name() {

		std::string server_name;

		if (this->current_token_.type != TT_VALUE)
		{
			std::cout << "Settings Parser: Invalid server_name argument" << std::endl;
			exit(1);
		}

		server_name = this->current_token_.value;
		this->advance();

		return server_name;
	}

	size_t SettingsParser::resolve_client_max_body_size() {
		size_t client_max_body_size;

		if (this->current_token_.type != TT_VALUE
			&& this->current_token_.value.find_first_not_of("0123456789") == std::string::npos) {
			std::cout << "Settings Parser: Invalid client_max_body_size argument" << std::endl;
			exit(1);
		}

		client_max_body_size = std::atoll(this->current_token_.value.c_str());
		this->advance();

		return client_max_body_size;
	}

	ErrorPage SettingsParser::resolve_error_page() {
		char *end_ptr = NULL;
		ErrorPage error_page;

		if (this->current_token_.type != TT_VALUE
			&& this->current_token_.value.find_first_not_of("0123456789") == std::string::npos)
		{
			std::cout << "Settings Parser: Invalid first error_page argument" << std::endl;
			exit(1);
		}

		error_page.code = std::strtoul(this->current_token_.value.c_str(), &end_ptr, 10);

		this->advance();

		if (this->current_token_.type != TT_VALUE)
		{
			std::cout << "Settings Parser: Invalid second error_page argument" << std::endl;
			exit(1);
		}

		error_page.path = this->current_token_.value;

		this->advance();

		return error_page;
	}

	std::string SettingsParser::resolve_root_element() {
		std::string root;

		if (this->current_token_.type != TT_VALUE)
		{
			std::cout << "Settings Parser: Invalid root arggument" << std::endl;
			exit(1);
		}
		
		root = this->current_token_.value;
		this->advance();

		return root;
	}

	std::string SettingsParser::resolve_index_element() {
		std::string index;

		if (this->current_token_.type != TT_VALUE)
		{
			std::cout << "Settings Parser: Invalid index arggument" << std::endl;
			exit(1);
		}
		
		index = this->current_token_.value;
		this->advance();

		return index;
	}

	bool SettingsParser::resolve_autoindex_element() {
		bool autoindex_value = false;
		
		if (this->current_token_.type != TT_VALUE 
			&& (this->current_token_.value != "on" || this->current_token_.value != "off"))
		{
			std::cout << "Settings Parser: Invalid index arggument" << std::endl;
			exit(1);
		}
		
		if (this->current_token_.value == "on")
			autoindex_value = true;

		this->advance();
		return autoindex_value;
	}

	std::vector<HTTP_METHOD> SettingsParser::resolve_accept_element() {
		std::vector<HTTP_METHOD> methods;

		while (this->pos_ != -1 && this->current_token_.type != TT_SEMICOLON) {

			if (this->current_token_.type == TT_VALUE && this->current_token_.value == "GET") {
				methods.push_back(GET);
			} else if (this->current_token_.type == TT_VALUE && this->current_token_.value == "POST") {
				methods.push_back(POST);
			} else if (this->current_token_.type == TT_VALUE && this->current_token_.value == "DELETE") {
				methods.push_back(DELETE);
			} else {
				std::cout << "Settings Parser: Invalid element in accept" << std::endl;
				exit(1);
			}

			this->advance();
		}

		if (methods.size() == 0) {
			std::cout << "Settings Parser: No parguments in accept element" << std::endl;
			exit(1);
		}

		return methods;
	}

	Rewrite SettingsParser::resolve_rewrite_element() {
		Rewrite rewrite;

		if (this->current_token_.type != TT_VALUE) {
			std::cout << "Settings Parser: No valid rewrite first argument" << std::endl;
			exit(1);
		}

		rewrite.from = this->current_token_.value;

		this->advance();

		if (this->current_token_.type != TT_VALUE) {
			std::cout << "Settings Parser: No valid rewrite second argument" << std::endl;
			exit(1);
		}

		rewrite.to = this->current_token_.value;

		this->advance();

		if (this->current_token_.type != TT_VALUE && this->current_token_.value != "permanent") {
			std::cout << "Settings Parser: No valid rewrite third argument" << std::endl;
			exit(1);
		}

		rewrite.permanent = true;
		this->advance();

		return rewrite;
	}

	Location SettingsParser::resolve_location_block() {
		Location location;
		
		if (current_token_.type != TT_VALUE) {
			std::cout << "Settings Parser: Invalid location argument" << std::endl;
			exit(1);
		}

		location.path = current_token_.value;

		this->advance();

		this->check_left_bracket();

		while (this->pos_ != -1 && this->current_token_.type != TT_RBRACKET) {

			if (this->current_token_.type == TT_ROOT) {
				this->advance();
				location.root = this->resolve_root_element();
				this->check_semicolon();
			} else if (this->current_token_.type == TT_INDEX) {
				this->advance();
				location.index = this->resolve_index_element();
				this->check_semicolon();
			} else if (this->current_token_.type == TT_AUTOINDEX) {
				this->advance();
				location.autoindex = this->resolve_autoindex_element();
				this->check_semicolon();
			} else if (this->current_token_.type == TT_ACCEPT) {
				this->advance();
				location.methods = this->resolve_accept_element();
				this->check_semicolon();
			} else if (this->current_token_.type == TT_REWRITE) {
				this->advance();
				location.rewrites.push_back(this->resolve_rewrite_element());
				this->check_semicolon();
			}
			else {
				std::cout << "Settings Parser: Invalid location block element" << std::endl;
				exit(1);
			}

		}

		this->check_right_bracket();

		return location;
	}

	ServerSettings SettingsParser::resolve_server_block() {

		ServerSettings server_settings;

		while (this->current_token_.type != TT_RBRACKET && this->pos_ != -1) {
			if (this->current_token_.type == TT_LISTEN) {
				this->advance();
				server_settings.port = this->resolve_listen_element();
				this->check_semicolon();
			} else if (this->current_token_.type == TT_SERVER_NAME ) {
				this->advance();
				server_settings.server_name = this->resolve_server_name();
				this->check_semicolon();
			} else if (this->current_token_.type == TT_CLIENT_MAX_BODY_SIZE) {
				this->advance();
				server_settings.client_max_body_size = this->resolve_client_max_body_size();
				this->check_semicolon();
			} else if (this->current_token_.type == TT_ERROR_PAGE) {
				this->advance();
				server_settings.error_pages.push_back(this->resolve_error_page());
				this->check_semicolon();
			} else if (this->current_token_.type == TT_LOCATION) {
				this->advance();
				server_settings.locations.push_back(this->resolve_location_block());
			} else {
				std::cout << "Settings Parser: Invalid server block element" << std::endl;
				exit(1);
			}
		}

		return server_settings;
	}

	Settings SettingsParser::parse() {

		if (current_token_.type != TT_SERVER) {
			std::cout << "Settings Parser: no server block" << std::endl;
			exit(1);
		}

		while (this->pos_ != -1) {
			if (this->current_token_.type == TT_SERVER) {
				this->advance();
				this->check_left_bracket();
				this->settings_.servers.push_back(this->resolve_server_block());
				this->check_right_bracket();
			}
		}

		return this->settings_;
	}

}  // namespace ws
