#include "http_parser.hpp"

#include <sstream>
#include <iostream>

#include "../server/connection.hpp"
#include "./http_request.hpp"

namespace ws
{

	HttpParser::HttpParser(DataBuffer &buff) {
		this->raw_request_ = std::string(buff.data, buff.size);
		this->valid_request_ = true;
	}

	std::string HttpParser::next_line() {
		std::string next_line;

		size_t new_line_pos = this->raw_request_.find("\n");
		next_line = this->raw_request_.substr(0, new_line_pos);

		if (new_line_pos == std::string::npos)
			this->raw_request_.erase(0, new_line_pos);
		else
			this->raw_request_.erase(0, new_line_pos + 1);

		return next_line;
	}

	std::vector<std::string> HttpParser::split_line(std::string line, char delimiter) {
		std::vector<std::string> tokens;
		std::string token;
		std::istringstream tokenStream(line);
		while (std::getline(tokenStream, token, delimiter)) {
			if (token.size() == 0)
				continue;
			tokens.push_back(token);
		}
		return tokens;
	}

	HTTP_METHOD HttpParser::resolve_request_method(std::string str) {
		if (str == "GET")
			return GET;
		else if (str == "POST")
			return POST;
		else if (str == "DELETE")
			return DELETE;
		return INVALID_METHOD;
	}

	std::string HttpParser::resolve_request_url(std::string str) {
		return str;
	}

	void HttpParser::parse_first_line() {
		std::string first_line = this->next_line();
		std::vector<std::string> split_line = this->split_line(first_line, ' ');

		if (split_line.size() != 3) {
			this->valid_request_ = false;
			return;
		}

		HTTP_METHOD parsed_method = resolve_request_method(split_line[0]);

		if (parsed_method == INVALID_METHOD) {
			this->valid_request_ = false;
			return;
		}

		this->request_.method = parsed_method;

		std::string parsed_url = resolve_request_url(split_line[1]);

		this->request_.url = parsed_url;

		if (split_line[2] != "HTTP/1.1") {
			this->valid_request_ = false;
			return;
		}

	}

	void HttpParser::parse_headers() {
		
		std::string current_header = this->next_line();
		while (current_header.size() != 0) {
			std::vector<std::string> split_header = this->split_line(current_header, ':');
			
			if (split_header.size() != 2) {
				this->valid_request_ = false;
				return;
			}

			std::string header_key = split_header[0];
			std::string header_value = split_header[1].substr(split_header[1].find_first_not_of(" "));

			this->request_.headers.insert(std::make_pair(header_key, header_value));

			current_header = this->next_line();
		}

	}

	void HttpParser::parse_body() {
		HttpRequest::headers_iterator element = this->request_.headers.find("Content-Length");

		if (element == this->request_.headers.end()) {
			return ;
		}

		this->request_.body = this->raw_request_;
	}

	HttpRequest HttpParser::parse() {
		this->parse_first_line();

		if (this->valid_request_ == false) {
			return this->request_;
		}

		this->parse_headers();

		if (this->valid_request_ == false) {
			return this->request_;
		}

		this->parse_body();

		if (this->valid_request_ == false) {
			return this->request_;
		}

		return this->request_;
	}

	bool HttpParser::request_is_valid() {
		return this->valid_request_;
	}

} // namespace ws
