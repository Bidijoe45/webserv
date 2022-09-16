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
		std::vector<std::string> header_block;
		std::string line = this->buff_.get_next_line();

		while(line.size() != 0)
		{
			header_block.push_back(line);
			line = this->buff_.get_next_line();
		}

		HttpHeaderParser header_parser(header_block);
		this->request_.headers = header_parser.parse_block();

		if (!header_parser.is_valid())
			throw std::runtime_error("Request: Invalid header block");
	}

	void HttpParser::parse_body()
	{
		this->request_.body = this->buff_.flush(this->buff_.size());
	}

	HttpRequest HttpParser::parse()
	{
		try
		{
			this->parse_first_line();
			this->parse_headers();
			this->parse_body();
		}
		catch(const std::runtime_error& e)
		{
			this->request_.is_valid = false;
			std::cout << e.what() << std::endl;
		}

		return this->request_;
	}

} // namespace ws
