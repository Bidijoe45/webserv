#include <iostream>

#include <string>

#include "http_request_line_parser.hpp"
#include "http_request_line.hpp"
#include "http_uri_parser.hpp"
#include "../server/data_buffer.hpp"
#include "../utils/string_utils.hpp"

namespace ws
{
    HttpRequestLineParser::HttpRequestLineParser(const std::string &line)
    {
        this->line_ = line;
        this->pos_ = 0;
        this->valid_ = true;
    }

    void HttpRequestLineParser::advance(size_t n = 1)
	{
	    if ((this->pos_ + n) >= this->line_.size())
	        throw std::runtime_error("Request Line Parser: trying to advance past the end of string");
		this->pos_ += n;
	}

    void HttpRequestLineParser::parse_method()
    {
    	size_t space_pos = this->line_.find_first_of(' ', this->pos_);
		std::string method = this->line_.substr(this->pos_, space_pos);

		if (method == "GET")
			this->request_line_.method = HTTP_METHOD_GET;
		else if (method == "POST")
			this->request_line_.method = HTTP_METHOD_POST;
		else if (method == "DELETE")
			this->request_line_.method = HTTP_METHOD_DELETE;
		else {
			this->request_line_.method = HTTP_METHOD_INVALID;
			throw std::runtime_error("Request Line Parser: invalid method");
		}

		this->advance(method.size());
    }

    void HttpRequestLineParser::check_space()
    {
		if (this->line_[this->pos_] != ' ')
			throw std::runtime_error("Request Line Parser: <Space> was expected");
		if (this->line_[this->pos_ + 1] == ' ')
			throw std::runtime_error("Request Line Parser: <LWR> space found");
		this->advance();
    }

    void HttpRequestLineParser::parse_uri()
    {
		size_t space_pos = this->line_.find_first_of(' ', this->pos_);
        
        if (space_pos == std::string::npos)
			throw std::runtime_error("Request Line Parser: <Space> was expected");

		std::string uri = this->line_.substr(this->pos_, space_pos - this->pos_);

		HttpUriParser uri_parser(uri);
		HttpUri parsed_uri = uri_parser.parse();

		if (!uri_parser.uri_is_valid())
			throw std::runtime_error("Request Line Parser: invalid URI");

		this->request_line_.uri = parsed_uri;
		this->advance(uri.size());
    }

    void HttpRequestLineParser::parse_version()
    {
		std::string version = this->line_.substr(this->pos_);

		if (version != "HTTP/1.1")
			throw std::runtime_error("Request Line Parser: invalid version");

		this->request_line_.http_version = version;
    }

    bool HttpRequestLineParser::is_valid()
    {
        return this->valid_;
    }

    HttpRequestLine HttpRequestLineParser::parse()
    {
        try
        {
		    if (!is_string_printable(this->line_, this->line_.size()))
			    throw std::runtime_error("Request Line Parser: non-printable characters in first line");
            this->parse_method();
    		this->check_space();
    		this->parse_uri();
    		this->check_space();
    		this->parse_version();
        }
        catch (const std::runtime_error &e)
        {
            this->valid_ = false;
            std::cout << e.what() << std::endl;
        }

        return this->request_line_;
    }
}
