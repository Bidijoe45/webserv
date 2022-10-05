#include "http_parser.hpp"

#include <sstream>
#include <iostream>
#include <exception>
#include <utility>
#include <algorithm>

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

	HttpParser::HttpParser() : stage_(HttpParser::REQUEST_LINE), buff_pos_(0), expected_body_size_(0), must_close_(false) {}

	void HttpParser::parse_first_line()
	{
		size_t found = this->buff_.find("\r\n", buff_pos_);
		if (found == std::string::npos)
		{
			if (this->buff_.size() > 0)
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
			this->throw_with_error(HttpRequest::BAD_REQUEST, "Request: Invalid first line");

		this->stage_ = HttpParser::HEADERS_BLOCK;
	}

	void HttpParser::parse_headers()
	{
		size_t found = this->buff_.find("\r\n\r\n", buff_pos_);
		if (found == std::string::npos)
		{
			if (this->buff_.size() >= 3)
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
			this->throw_with_error(HttpRequest::BAD_REQUEST, "Request: Invalid header block");

		HttpHeaderMap::iterator transfer_encoding_it = this->request_.headers.find(HTTP_HEADER_TRANSFER_ENCODING);
		if (transfer_encoding_it != this->request_.headers.end())
		{
			HttpHeaderTransferEncoding *transfer_encoding_header = static_cast<HttpHeaderTransferEncoding *>(transfer_encoding_it->second);
			this->transfer_codings_ = transfer_encoding_header->codings;
			this->request_.headers.erase(transfer_encoding_it);
			if (this->transfer_codings_.size() > 0)
			{
				this->stage_ = HttpParser::CHUNKED_BODY;
				return;
			}
		}
		HttpHeaderMap::iterator content_length_it = this->request_.headers.find(HTTP_HEADER_CONTENT_LENGTH);
		if (content_length_it != this->request_.headers.end())
		{
			this->stage_ = HttpParser::SIMPLE_BODY;
			HttpHeaderContentLength *length = static_cast<HttpHeaderContentLength *>(content_length_it->second);
			this->expected_body_size_ = length->content_length;
		}
		else
		{
			this->stage_ = HttpParser::COMPLETED;
			if (this->buff_.size() > 0)
			{
				this->must_close_ = true;
				this->throw_with_error(HttpRequest::LENGTH_REQUIRED, "Request: body present but missing content-length");
			}
		}
	}

	void HttpParser::parse_body()
	{
		if (this->buff_.size() > 0 && this->expected_body_size_ > 0)
		{
			if (this->buff_.size() < this->expected_body_size_)
				return;

			this->request_.body = this->buff_.flush(this->expected_body_size_);
		}
		this->stage_ = HttpParser::COMPLETED;
	}

	void HttpParser::parse_chunked_body()
	{
		if (this->transfer_codings_.size() == 0)
			this->throw_with_error(HttpRequest::BAD_REQUEST, "Request: empty transfer encoding");

		if (this->transfer_codings_.back() != "chunked")
		{
			this->must_close_ = true;
			this->throw_with_error(HttpRequest::BAD_REQUEST, "Request: last transfer coding must be chunked");
		}

		if (this->transfer_codings_.size() != 1)
			this->throw_with_error(HttpRequest::NOT_IMPLEMENTED, "Request: unimplemented transfer coding or more than one chunked");

		HttpHeaderMap::iterator content_length = this->request_.headers.find(HTTP_HEADER_CONTENT_LENGTH);
		if (content_length != this->request_.headers.end())
		{
			this->request_.headers.erase(content_length);
			this->must_close_ = true;
		}

		std::string raw_body = this->buff_.flush(this->buff_.size());
		this->unchunker_.unchunk(raw_body);
		if (this->unchunker_.get_stage() == Unchunker::COMPLETED)
		{
			this->stage_ = HttpParser::COMPLETED;
			this->request_.body = this->unchunker_.get_unchunked_body();
			if (this->unchunker_.is_valid() == false)
			{
				this->must_close_ = true;
				this->throw_with_error(HttpRequest::BAD_REQUEST, "Request: invalid chunked body");
			}
		}
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
		this->stage_ = HttpParser::REQUEST_LINE;
		this->expected_body_size_ = 0;
		this->transfer_codings_.clear();
		this->unchunker_.reset();
	}

	bool HttpParser::must_close() const
	{
		return this->must_close_;
	}

	void HttpParser::throw_with_error(HttpRequest::RequestError error, const std::string &msg)
	{
		this->request_.error = error;
		throw std::runtime_error(msg);
	}

	void HttpParser::parse(const DataBuffer &new_buff)
	{
		this->buff_.append(new_buff.data); 
		this->must_close_ = false;
		try
		{
			if (this->stage_ == HttpParser::REQUEST_LINE)
				this->parse_first_line();
			if (this->stage_ == HttpParser::HEADERS_BLOCK)
				this->parse_headers();
			if (this->stage_ == HttpParser::SIMPLE_BODY)
				this->parse_body();
			if (this->stage_ == HttpParser::CHUNKED_BODY)
				this->parse_chunked_body();
		}
		catch(const std::runtime_error& e)
		{
			this->request_.is_valid = false;
			this->stage_ = HttpParser::COMPLETED;
			std::cout << e.what() << std::endl;
		}
	}

} // namespace ws
