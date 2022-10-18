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

	HttpParser::HttpParser() : stage_(HttpParser::REQUEST_LINE), buff_pos_(0), expected_body_size_(0), must_close(false), max_body_size_(0) {}

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
			this->complete_with_error(HttpRequest::BAD_REQUEST, "Request: Invalid first line (BAD REQUEST)");

		if (this->request_.request_line.http_version != "HTTP/1.1")
			this->complete_with_error(HttpRequest::INVALID_VERSION, "Request: Invalid first line (INVALID VERSION)");

		const std::string &absolute_path = this->request_.request_line.uri.absolute_path();
		if (absolute_path.size() > URI_MAX_LENGTH)
			this->complete_with_error(HttpRequest::URI_TOO_LONG, "Request: Invalid first line (URI TOO LONG)");

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
			this->complete_with_error(HttpRequest::BAD_REQUEST, "Request: Invalid header block");
		if (this->request_.headers.find(HTTP_HEADER_HOST) == this->request_.headers.end())
			this->complete_with_error(HttpRequest::BAD_REQUEST, "Request: missing Host header");

		HttpHeaderMap::iterator transfer_encoding_it = this->request_.headers.find(HTTP_HEADER_TRANSFER_ENCODING);
		if (transfer_encoding_it != this->request_.headers.end())
		{
			HttpHeaderTransferEncoding *transfer_encoding_header = static_cast<HttpHeaderTransferEncoding *>(transfer_encoding_it->second);
			this->transfer_codings_ = transfer_encoding_header->codings;
			this->request_.headers.erase(transfer_encoding_it);
			this->stage_ = HttpParser::CHUNKED_BODY;
			return;
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
				this->must_close = true;
				this->complete_with_error(HttpRequest::LENGTH_REQUIRED, "Request: body present but missing content-length");
			}
		}
	}

	void HttpParser::parse_body()
	{
		if (this->buff_.size() > 0 && this->expected_body_size_ > 0)
		{
			std::cout << "MAX_BODY: " << this->max_body_size_ << std::endl;
			if (this->max_body_size_ != 0 && this->expected_body_size_ > this->max_body_size_)
				this->complete_with_error(HttpRequest::BODY_TOO_LARGE, "Request: body too large");
			if (this->buff_.size() < this->expected_body_size_)
				return;

			this->request_.body = this->buff_.flush(this->expected_body_size_);
		}
		this->stage_ = HttpParser::COMPLETED;
	}

	void HttpParser::parse_chunked_body()
	{
		if (this->transfer_codings_.back() != "chunked")
		{
			this->must_close = true;
			this->complete_with_error(HttpRequest::BAD_REQUEST, "Request: last transfer coding must be chunked");
		}

		if (this->transfer_codings_.size() != 1)
			this->complete_with_error(HttpRequest::NOT_IMPLEMENTED, "Request: unimplemented transfer coding or more than one chunked");

		HttpHeaderMap::iterator content_length = this->request_.headers.find(HTTP_HEADER_CONTENT_LENGTH);
		if (content_length != this->request_.headers.end())
		{
			this->request_.headers.erase(content_length);
			this->must_close = true;
		}

		std::string raw_body = this->buff_.flush(this->buff_.size());
		this->unchunker_.unchunk(raw_body);
		if (this->unchunker_.get_stage() == Unchunker::COMPLETED)
		{
			this->stage_ = HttpParser::COMPLETED;
			this->request_.body = this->unchunker_.get_unchunked_body();
			if (this->unchunker_.is_valid() == false)
			{
				this->must_close = true;
				this->complete_with_error(HttpRequest::BAD_REQUEST, "Request: invalid chunked body");
			}
		}
	}

	void HttpParser::set_max_body_size(size_t max_body_size)
	{
		this->max_body_size_ = max_body_size;
	}

	void HttpParser::append_to_buff(const DataBuffer &buff)
	{
		this->buff_.append(buff.data); 
	}

	const HttpRequest &HttpParser::get_request() const
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
		this->max_body_size_ = 0;
		this->transfer_codings_.clear();
		this->unchunker_.reset();
	}

	void HttpParser::complete_with_error(HttpRequest::RequestError error, const std::string &msg)
	{
		this->request_.error = error;
		std::cout << msg << std::endl;
		this->stage_ = HttpParser::COMPLETED;
	}

} // namespace ws
