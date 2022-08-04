#include "http_parser.hpp"

#include <sstream>
#include <iostream>
#include <exception>
#include <utility>

#include "http_request.hpp"
#include "../server/data_buffer.hpp"
#include "../utils/string_utils.hpp"
#include "http_uri.hpp"
#include "http_uri_parser.hpp"
#include "http_header.hpp"
#include "http_header_parser.hpp"
#include "http_header_map.hpp"

namespace ws
{

	HttpParser::HttpParser(DataBuffer &buff) : buff_(buff), line_pos_(0) {}

	std::string HttpParser::get_next_line()
	{
		std::string line;
		size_t crlf_pos = this->buff_.find("\r\n");

		if (crlf_pos == std::string::npos)
			line = this->buff_.flush(this->buff_.size());
		else
		{
			line = this->buff_.flush(crlf_pos);
			this->buff_.flush(2);
		}
		if (line.find('\r') != std::string::npos)
			throw (std::runtime_error("get_next_line: invalid line because lonely CR found"));

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
			this->request_.method = HTTP_METHOD_INVALID;
			this->request_.error = HTTP_REQUEST_INVALID_METHOD;
			throw std::runtime_error("Request: invalid method");
		}

		this->advance(method.size());
	}

	void HttpParser::parse_uri()
	{
		size_t space_pos = this->line_.find_first_of(' ', this->line_pos_);
		std::string uri = this->line_.substr(this->line_pos_, space_pos - this->line_pos_);

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
		this->line_ = this->get_next_line();
		if (this->line_.size() == 0)
			this->line_ = this->get_next_line();
		if (!is_string_printable(this->line_, this->line_.size()))
			throw std::runtime_error("Request: non-printable characters in first line");

		this->parse_method();
		this->check_space();
		this->parse_uri();
		this->check_space();
		this->parse_version();
		this->line_pos_ = 0;
	}

	void HttpParser::skipOWS()
	{
		this->line_pos_ = this->line_.find_first_not_of(" \t", this->line_pos_);
	}

	std::string HttpParser::get_header_name()
	{
		std::string	header_name;
		size_t		colon_pos = this->line_.find_first_of(':', this->line_pos_);

		header_name = this->line_.substr(this->line_pos_, colon_pos);
		header_name = string_to_lower(header_name, header_name.size());

		if (!is_token(header_name))
			throw std::runtime_error("Request: invalid header name characters");

		this->advance(colon_pos + 1);
		return header_name;
	}

	std::string	HttpParser::get_header_value()
	{
		std::string	header_value;
		size_t	value_end = this->line_.find_last_not_of(" \t");

		header_value = this->line_.substr(this->line_pos_, value_end - this->line_pos_ + 1);
		for (size_t i = 0; header_value[i]; i++)
		{
			if (!std::isprint(header_value[i])
				&& header_value[i] != ' ' && header_value[i] != '\t'
				&& !is_obstext(header_value[i]))
				throw std::runtime_error("Request: invalid header value characters");
		}
		return header_value;
	}

	void HttpParser::parse_headers()
	{
		std::string				header_name;
		std::string				header_value;
		HttpHeaderParser		header_line_parser;
		HttpHeader				*parsed_header;
		HttpHeaderMap::iterator	found_header;

		this->line_ = this->get_next_line();
		while (this->line_.size() != 0)
		{
			this->line_pos_ = 0;
			header_name = get_header_name();
			this->skipOWS();
			header_value = get_header_value();
			found_header = this->request_.headers.find(header_name);
			if (found_header != this->request_.headers.end())
				this->request_.headers.combine_value(found_header, header_value);
			else
			{
				parsed_header = header_line_parser.parse(header_name, header_value);
				this->request_.headers.insert(header_name, parsed_header);
			}
			this->line_ = this->get_next_line();
		}	
	}

	HttpRequest HttpParser::parse()
	{
		this->valid_request_ = true;
		try
		{
			this->parse_first_line();
			this->parse_headers();
			// utilizar los datos parseados para determinar si se espera un body o no
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
