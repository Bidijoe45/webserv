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
#include "http_headers.hpp"
#include "http_header_parser.hpp"
#include "http_header_map.hpp"
#include "http_request_line_parser.hpp"

namespace ws
{

	HttpParser::HttpParser() : stage_(REQUEST_LINE), buff_pos_(0), current_body_size_(0), expected_body_size_(-1) {}

	void HttpParser::parse_first_line()
	{
		size_t found = this->buff_.find("\r\n", buff_pos_);
		if (found == std::string::npos)
		{
			this->buff_pos_ = this->buff_.size() - 1;
			return;
		}
		this->buff_pos_ = 0;

		std::string line = this->buff_.get_next_line();
		if (line.size() == 0)
			line = this->buff_.get_next_line();

		HttpRequestLineParser request_line_parser(line);
		this->request_.request_line = request_line_parser.parse();

		if (!request_line_parser.is_valid())
			throw std::runtime_error("Request: Invalid first line");
		this->stage_ = HEADERS_BLOCK;
	}

	void HttpParser::parse_headers()
	{
		size_t found = this->buff_.find("\r\n\r\n", buff_pos_);
		if (found == std::string::npos)
		{
			this->buff_pos_ = this->buff_.size() - 3;
			return;
		}
		this->buff_pos_ = 0;

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

		HttpHeaderMap::iterator transfer_encoding_it = this->request_.headers.find(HTTP_HEADER_TRANSFER_ENCODING);
		if (transfer_encoding_it == this->request_.headers.end())
		{
			this->stage_ = SIMPLE_BODY;
			HttpHeaderMap::iterator content_length_it = this->request_.headers.find(HTTP_HEADER_CONTENT_LENGTH);
			if (content_length_it != this->request_.headers.end())
			{
				HttpHeaderContentLength *length = static_cast<HttpHeaderContentLength *>(content_length_it->second);
				this->expected_body_size_ = length->content_length;
			}
			else
				this->expected_body_size_ = 0;
		}
		else
			this->stage_ = CHUNKED_BODY;
	}

	void HttpParser::parse_body()
	{
		if (this->expected_body_size_ != 0)
		{
			//TODO: añadir todas las posibilidades de content length
			this->request_.body += this->buff_.flush(this->buff_.size());
		}
		this->stage_ = COMPLETED;
	}

	HttpRequest HttpParser::get_request() const
	{
		return this->request_;
	}

	HttpParser::Stage HttpParser::get_stage() const
	{
		return this->stage_;
	}

	void HttpParser::reset()
	{
		this->buff_.clear();
		this->buff_pos_ = 0;
		this->request_.reset();
		this->stage_ = REQUEST_LINE;
	}

	void HttpParser::parse(const DataBuffer &new_buff)
	{
		this->buff_.append(new_buff.data); 
		try
		{
			if (this->stage_ == REQUEST_LINE)
				this->parse_first_line();
			if (this->stage_ == HEADERS_BLOCK)
				this->parse_headers();
			if (this->stage_ == SIMPLE_BODY)
				this->parse_body();
			//if (this->stage_ == CHUNKED_BODY)
			else
				return;
		}
		catch(const std::runtime_error& e)
		{
			this->request_.is_valid = false;
			this->stage_ = COMPLETED;
			std::cout << e.what() << std::endl;
		}
	}

} // namespace ws
