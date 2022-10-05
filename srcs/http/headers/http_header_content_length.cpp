#include <string>
#include <stdexcept>
#include <iostream>

#include "http_headers.hpp"
#include "string_utils.hpp"

namespace ws
{
	HttpHeaderContentLength::HttpHeaderContentLength()
	{
		this->type = HTTP_HEADER_CONTENT_LENGTH;
	}

	HttpHeaderContentLength::~HttpHeaderContentLength() {}
	
	HttpHeaderContentLength::HttpHeaderContentLength(const HttpHeaderContentLength &src)
	{
		this->type = src.type;
		this->value = src.value;
		this->content_length = src.content_length;
	}

	void HttpHeaderContentLength::parse_value()
	{
		try
		{
			this->content_length = std::stoul(this->value);
		}
		catch (const std::out_of_range &e)
		{
			this->is_valid = false;
			std::cout << "Content Length Header: " << e.what() << std::endl;
		}
		catch (const std::invalid_argument &e)
		{
			this->is_valid = false;
			std::cout << "Content Length Header: " << e.what() << std::endl;
		}
	}

	void HttpHeaderContentLength::set_value(const std::string &value)
	{
		this->value = value;
		this->parse_value();
	}

	void HttpHeaderContentLength::set_value(const size_t &value)
	{
		this->value = ul_to_string(value);
		this->content_length = value;
	}

}
