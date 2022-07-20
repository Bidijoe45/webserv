#include "http_parser.hpp"

#include <sstream>
#include <iostream>
#include <exception>

#include "../server/connection.hpp"
#include "./http_request.hpp"

namespace ws
{

	HttpParser::HttpParser(DataBuffer &buff) : buff_(buff) {}

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

	void HttpParser::parse_first_line()
	{
		size_t element_i = 0;
		size_t sp_pos = 0;
		size_t i = 0;
		std::string	element;

		while (element_i < 2)
		{
			sp_pos = this->line_.find_first_of(' ', i);
			if (sp_pos == std::string::npos)
				throw std::runtime_error("Request: error in first line");
			element = this->line_.substr(i, sp_pos);
			if (element_i == 0)
				this->request_.set_method(element);
			else
				this->request_.url = element;
			i = sp_pos + 1;
			if (line_[i] == ' ')
				throw std::runtime_error("Request: error in first line");
			element_i++;
		}
		this->request_.http_version = this->line_.substr(i);
	}

	HttpRequest HttpParser::parse()
	{
		this->line_ = this->get_next_line();

		try
		{
			this->parse_first_line();
			this->parse_headers();
			this->parse_body();
		}
		catch(const std::runtime_error& e)
		{
			valid_request_ = false;
		}
	}

	bool HttpParser::request_is_valid()
	{
		return this->valid_request_;
	}

} // namespace ws
