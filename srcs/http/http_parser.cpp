#include "http_parser.hpp"

#include <sstream>
#include <iostream>
#include <exception>

#include "./http_request.hpp"
#include "../server/data_buffer.hpp"
#include "../utils/string_utils.hpp"
#include "http_uri.hpp"
#include "http_uri_parser.hpp"

namespace ws
{

	HttpParser::HttpParser(DataBuffer &buff) : buff_(buff), line_pos_(0){}

	std::string HttpParser::get_next_line()
	{
		std::string line;
		size_t crlf_pos = this->buff_.find("\r\n");

		if (crlf_pos == std::string::npos)
			return this->buff_.flush(this->buff_.size());

		line = this->buff_.flush(crlf_pos);
		this->buff_.flush(2);

		return line;
	}

	void HttpParser::advance(size_t n = 1)
	{
		this->line_pos_ += n;
	}

	void HttpParser::check_space()
	{
		if (this->line_[this->line_pos_] != ' ')
			throw std::runtime_error("Request: <Space> was expected");
		if (this->line_[this->line_pos_ + 1] == ' ')
			throw std::runtime_error("Request: <LWR> space found");
		this->advance();
	}

	void HttpParser::parse_method()
	{
		size_t space_pos = this->line_.find_first_of(' ', this->line_pos_);
		std::string method = this->line_.substr(this->line_pos_, space_pos);

		if (method == "GET")
			this->request_.method = HTTP_METHOD_GET;
		else if (method == "POST")
			this->request_.method = HTTP_METHOD_POST;
		else if (method == "DELETE")
			this->request_.method = HTTP_METHOD_DELETE;
		else {
			this->request_.error = HTTP_REQUEST_INVALID_METHOD;
			throw std::runtime_error("Request: invalid method");
		}

		this->advance(method.size());
	}

	void HttpParser::parse_uri()
	{
		size_t space_pos = this->line_.find_first_of(' ', this->line_pos_);
		std::string uri = this->line_.substr(this->line_pos_, space_pos - line_pos_);

		HttpUriParser uri_parser(uri);
		HttpUri parsed_uri = uri_parser.parse();

		if (!uri_parser.uri_is_valid())
		{
			this->request_.error = HTTP_REQUEST_INVALID_URI;
			throw std::runtime_error("Request: invalid URI");
		}
		this->request_.uri = parsed_uri;
		this->advance(uri.size());
	}

	void HttpParser::parse_version()
	{
		std::string version = this->line_.substr(this->line_pos_);

		if (version != "HTTP/1.1")
		{
			this->request_.error = HTTP_REQUEST_INVALID_VERSION;
			throw std::runtime_error("Request: invalid version");
		}

		this->request_.http_version = version;
	}

	void HttpParser::parse_first_line()
	{
		if (!is_string_printable(this->line_, this->line_.size()))
			throw std::runtime_error("Request: no printable characters in first line");

		this->parse_method();
		this->check_space();
		this->parse_uri();
		this->check_space();
		this->parse_version();
		this->line_pos_ = 0;
	}

	HttpRequest HttpParser::parse()
	{
		this->line_ = this->get_next_line();
		this->valid_request_ = true;
		try
		{
			this->parse_first_line();
			//this->parse_headers();
			//this->parse_body();
		}
		catch(const std::runtime_error& e)
		{
			this->valid_request_ = false;
			std::cout << e.what() << std::endl;
		}

		return this->request_;
	}

	bool HttpParser::request_is_valid()
	{
		return this->valid_request_;
	}

} // namespace ws
