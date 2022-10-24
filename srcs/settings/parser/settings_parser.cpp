#include <string>
#include <exception>

#include "settings_parser.hpp"
#include "../settings.hpp"
#include "../lexer/settings_lexer.hpp"
#include "../lexer/token.hpp"
#include "../cgi_settings.hpp"
#include "../../http/http_uri_parser.hpp"
#include "../../utils/string_utils.hpp"

namespace ws {

	SettingsParser::SettingsParser(std::string settings_file) : settings_file_(settings_file) {
		SettingsLexer lexer = SettingsLexer(this->settings_file_);
		if(!lexer.file_is_valid())
		{
			this->valid_file_ = false;
			this->error_msg_ = lexer.get_error_msg();
			return ;
		}
		this->tokens_ = lexer.make_tokens();
		this->n_tokens_ = this->tokens_.size();
		this->pos_ = 0;
		this->valid_file_ = true;
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
			throw std::runtime_error(std::string("Missing semicolon"));
		}

		this->advance();
	}

	void SettingsParser::check_left_bracket() {
		
		if (this->current_token_.type != TT_LBRACKET){
			throw std::runtime_error(std::string("Missing left bracket"));
		}

		this->advance();
	}

	void SettingsParser::check_right_bracket() {
		if (this->current_token_.type != TT_RBRACKET){
			throw std::runtime_error(std::string("Missing right bracket"));
		}

		this->advance();
	}

	size_t SettingsParser::resolve_listen_element() {
		size_t port_n;

		if (this->current_token_.type != TT_VALUE || !str_is_numeric(this->current_token_.value))
		{
			throw std::runtime_error(std::string("Invalid listen argument"));
		}

		port_n = std::atoll(this->current_token_.value.c_str());
		this->advance();

		return port_n;
	}

	std::string SettingsParser::resolve_server_name() {

		std::string server_name;

		if (this->current_token_.type != TT_VALUE)
		{
			throw std::runtime_error(std::string("Invalid server_name argument"));
		}

		server_name = this->current_token_.value;
		this->advance();

		return server_name;
	}

	size_t SettingsParser::resolve_client_max_body_size() {
		size_t client_max_body_size;

		if (this->current_token_.type != TT_VALUE || !str_is_numeric(this->current_token_.value))
		{
			throw std::runtime_error(std::string("Invalid client_max_body_size argument"));
		}

		client_max_body_size = std::atoll(this->current_token_.value.c_str());
		this->advance();

		return client_max_body_size;
	}

	ErrorPage SettingsParser::resolve_error_page() {
		char *end_ptr = NULL;
		ErrorPage error_page;

		if (this->current_token_.type != TT_VALUE || !str_is_numeric(this->current_token_.value))
		{
			throw std::runtime_error(std::string("Invalid first error_page argument"));
		}

		error_page.code = std::strtoul(this->current_token_.value.c_str(), &end_ptr, 10);

		this->advance();

		if (this->current_token_.type != TT_VALUE)
		{
			throw std::runtime_error(std::string("Invalid second error_page argument"));
		}

		error_page.path = this->current_token_.value;

		this->advance();

		return error_page;
	}

	std::string SettingsParser::resolve_root_element() {
		std::string root;

		if (this->current_token_.type != TT_VALUE)
		{
			throw std::runtime_error(std::string("Invalid root arggument"));
		}
		
		root = this->current_token_.value;
		this->advance();

		return root;
	}

	std::string SettingsParser::resolve_index_element() {
		std::string index;

		if (this->current_token_.type != TT_VALUE)
		{
			throw std::runtime_error(std::string("Invalid index argument"));
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
			throw std::runtime_error(std::string("Invalid autoindex argument"));
		}

		if (this->current_token_.value == "on")
			autoindex_value = true;

		this->advance();
		return autoindex_value;
	}

	std::vector<HTTP_METHOD> SettingsParser::resolve_accept_element() {
		std::vector<HTTP_METHOD> methods;

		while (this->current_token_.type != TT_END && this->current_token_.type != TT_SEMICOLON) {

			if (this->current_token_.type == TT_VALUE && this->current_token_.value == "GET") {
				methods.push_back(HTTP_METHOD_GET);
			} else if (this->current_token_.type == TT_VALUE && this->current_token_.value == "POST") {
				methods.push_back(HTTP_METHOD_POST);
			} else if (this->current_token_.type == TT_VALUE && this->current_token_.value == "DELETE") {
				methods.push_back(HTTP_METHOD_DELETE);
			} else {
				throw std::runtime_error(std::string("Invalid element in accept"));
			}

			this->advance();
		}

		if (methods.size() == 0) {
			throw std::runtime_error(std::string("No arguments in accept element"));
		}

		return methods;
	}

	Redirect SettingsParser::resolve_redirect_element() {
		Redirect redirect;

		if (this->current_token_.type != TT_VALUE || !str_is_numeric(this->current_token_.value)) {
			throw std::runtime_error(std::string("No valid redirect first argument"));
		}

		redirect.code = atoll(this->current_token_.value.c_str());

		this->advance();

		if (this->current_token_.type != TT_VALUE) {
			throw std::runtime_error(std::string("No valid redirect second argument"));
		}

		HttpUriParser uri_parser(this->current_token_.value);
		redirect.to = uri_parser.parse();

		if (!uri_parser.uri_is_valid())
			throw std::runtime_error(std::string("No valid redirect uri"));

		this->advance();

		return redirect;
	}

	std::string SettingsParser::resolve_upload_element()
	{
		std::string upload_dir = this->current_token_.value;
		this->advance();

		return upload_dir;
	}

	Location SettingsParser::resolve_location_block() {
		Location location;
		
		if (current_token_.type != TT_VALUE) {
			throw std::runtime_error(std::string("Invalid location argument"));
		}

		location.path = current_token_.value;

		this->advance();

		this->check_left_bracket();

		while (this->current_token_.type != TT_END && this->current_token_.type != TT_RBRACKET) {

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
			} else if (this->current_token_.type == TT_REDIRECT) {
				this->advance();
				location.redirect = this->resolve_redirect_element();
				this->check_semicolon();
			} else if (this->current_token_.type == TT_CGI) {
				this->advance();
				location.cgis.push_back(this->resolve_cgi_element());
				this->check_semicolon();
			} else if (this->current_token_.type == TT_UPLOAD) {
				this->advance();
				location.upload_dir = this->resolve_upload_element();
				this->check_semicolon();
			} else {
				throw std::runtime_error(std::string("Invalid location block element"));
			}

		}

		this->check_right_bracket();

		return location;
	}

	CGISettings SettingsParser::resolve_cgi_element()
	{
		CGISettings cgi_settings;

		if (this->current_token_.type != TT_VALUE)
			throw std::runtime_error(std::string("Missing CGI extension"));

		cgi_settings.extension = this->current_token_.value;

		this->advance();
		
		if (this->current_token_.type != TT_VALUE)
			throw std::runtime_error(std::string("Missing CGI executable"));

		cgi_settings.executable = this->current_token_.value;

		this->advance();

		return cgi_settings;
	}

	ServerSettings SettingsParser::resolve_server_block() {

		ServerSettings server_settings;

		server_settings.client_max_body_size = 0;

		while (this->current_token_.type != TT_RBRACKET && this->current_token_.type != TT_END) {
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
				throw std::runtime_error(std::string("Invalid server block element"));
			}
		}

		return server_settings;
	}

	bool SettingsParser::is_valid() {
		return this->valid_file_;
	}

	std::string SettingsParser::get_error_msg() {
		return this->error_msg_;
	}

	Settings SettingsParser::parse() {
		if (!this->valid_file_)
			return this->settings_;
		if (this->n_tokens_ == 0)
		{
			this->valid_file_ = false;
			return this->settings_;
		}

		this->current_token_ = this->tokens_[0];

		try {
			if (this->current_token_.type != TT_SERVER) {
				throw std::runtime_error(std::string("Invalid server block element"));
			}
			
			while (this->current_token_.type != TT_END) {
				if (this->current_token_.type != TT_SERVER) 
					break ;

				this->advance();
				this->check_left_bracket();
				this->settings_.servers.push_back(this->resolve_server_block());
				this->check_right_bracket();
			}

		} catch (std::runtime_error &e) {
			this->error_msg_ = std::string(e.what());
			this->valid_file_ = false;
		}
		
		return this->settings_;
	}

}  // namespace ws
