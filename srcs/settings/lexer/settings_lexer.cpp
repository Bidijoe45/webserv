#include <string>
#include <fstream>
#include <vector>
#include <map>

#include "settings_lexer.hpp"
#include "token.hpp"

namespace ws
{
	SettingsLexer::SettingsLexer(std::string file_name) {

		std::ifstream file(file_name);
		std::string file_str;

		if (!file || !file.is_open()) {
			this->valid_file_ = false;
			this->error_msg_ = "Could not open the file";
			return ;
		}

		while (std::getline(file, file_str)) {
			this->str_ += file_str;
		}

		this->pos_ = -1;
		this->current_char_ = 0;
		this->str_len_ = this->str_.size();
		this->valid_file_ = true;
	}

	bool SettingsLexer::file_is_valid() {
		return this->valid_file_;
	}

	std::string SettingsLexer::get_error_msg() {
		return this->error_msg_;
	}

	void SettingsLexer::advance() {
		if (this->pos_ == this->str_len_ - 1) {
			this->current_char_ = -1;
			return ;
		}

		this->pos_++;
		this->current_char_ = this->str_[this->pos_];
	}

	std::vector<Token> SettingsLexer::make_tokens() {
		
		std::vector<Token> tokens;
		
		this->advance();

		while (this->current_char_ != -1) {

			if (std::isspace(current_char_)) {
				this->advance();
			} else if (this->current_char_ == '{') {
				tokens.push_back(Token(TT_LBRACKET));
				this->advance();
			} else if (this->current_char_ == '}') {
				tokens.push_back(Token(TT_RBRACKET));
				this->advance();
			} else if (this->current_char_ == ';') {
				tokens.push_back(Token(TT_SEMICOLON));
				this->advance();
			} else {
				tokens.push_back(this->make_value());
			}

		}

		return tokens;
	}

	Token SettingsLexer::resolve_value_type(const std::string &value) {

		if (value == "server") {
			return Token(TT_SERVER);
		} else if (value == "location") {
			return Token(TT_LOCATION);
		} else if (value == "server_name") {
			return Token(TT_SERVER_NAME);
		} else if (value == "error_page") {
			return Token(TT_ERROR_PAGE);
		} else if (value == "client_max_body_size") {
			return Token(TT_CLIENT_MAX_BODY_SIZE);
		} else if (value == "root") {
			return Token(TT_ROOT);
		} else if (value == "index") {
			return Token(TT_INDEX);
		} else if (value == "accept") {
			return Token(TT_ACCEPT);
		} else if (value == "rewrite") {
			return Token(TT_REWRITE);
		} else if (value == "autoindex") {
			return Token(TT_AUTOINDEX);
		} else if (value == "listen") {
			return Token(TT_LISTEN);
		} else if (value == "cgi") {
			return Token(TT_CGI);
		} else if (value == "upload") {
			return Token(TT_UPLOAD);
		} 

		return Token(TT_VALUE, value);
	}

	Token SettingsLexer::make_value() {
		std::string value;
		TOKEN_TYPE value_type;

		while (this->current_char_ != -1 && !std::isspace(this->current_char_) && this->current_char_ != '{'
				&& this->current_char_ != '}' && this->current_char_ != ';') {
			value += this->current_char_;
			this->advance();
		}

		return resolve_value_type(value);
	}

} // namespace ws
