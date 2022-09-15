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
#include "http_request_line_parser.hpp"

namespace ws
{

	HttpParser::HttpParser(DataBuffer &buff) : buff_(buff) {}

	void HttpParser::parse_first_line()
	{
		std::string line = this->buff_.get_next_line();
		if (line.size() == 0)
			line = this->buff_.get_next_line();

		HttpRequestLineParser request_line_parser(line);
		this->request_.request_line = request_line_parser.parse();

		if (!request_line_parser.is_valid())
			throw std::runtime_error("Request: Invalid first line");
	}

	void HttpParser::parse_headers()
	{
		std::string header_block_string;
		std::string last_str;
		header_block_string = this->buff_.get_next_line();
		last_str = header_block_string;
		while(last_str != "\r\n")
		{
			last_str = this->buff_.get_next_line();
			header_block_string += last_str;
		}
		DataBuffer header_block_buff(header_block_string);
		HttpHeaderParser header_parser(header_block_buff);
		if (!header_parser.is_valid())
			throw std::runtime_error("Request: Invalid header block");
		this->request_.headers = header_parser.parse_block();
	}

	void HttpParser::parse_body()
	{
		this->request_.body = this->buff_.flush(this->buff_.size());
	}

	HttpRequest HttpParser::parse()
	{
		this->valid_request_ = true;
		try
		{
			this->parse_first_line();
			this->parse_headers();
			this->parse_body();
		}
		catch(const std::runtime_error& e)
		{
			this->valid_request_ = false;
			std::cout << e.what() << std::endl;
			if (this->request_.error == HTTP_REQUEST_NO_ERROR)
				this->request_.error = HTTP_REQUEST_OTHER_SYNTAX_ERROR;
		}

		return this->request_;
	}

	bool HttpParser::request_is_valid()
	{
		return this->valid_request_;
	}

} // namespace ws
