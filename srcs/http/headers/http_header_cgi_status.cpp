#include <string>

#include "http_headers.hpp"

namespace ws
{
	HttpHeaderCGIStatus::HttpHeaderCGIStatus()
	{
		this->type = HTTP_HEADER_CGI_STATUS;
		this->status_code = -1;
	}

	HttpHeaderCGIStatus::~HttpHeaderCGIStatus() {}
	
	HttpHeaderCGIStatus::HttpHeaderCGIStatus(const HttpHeaderCGIStatus &src)
	{
		this->type = src.type;
		this->value = src.value;
		this->status_code = src.status_code;
		this->reason_phrase = src.reason_phrase;
	}

	void HttpHeaderCGIStatus::parse_value()
	{
		size_t pos = this->value.find(" ");	
		try
		{
			this->status_code = std::stoul(this->value.substr(0, pos));
			pos++;
			if (pos == std::string::npos)
				return;
			this->reason_phrase = this->value.substr(pos);
		}
		catch (const std::out_of_range &e)
		{
			this->is_valid = false;
			std::cout << "CGI Status Header: " << e.what() << std::endl;
		}
		catch (const std::invalid_argument &e)
		{
			this->is_valid = false;
			std::cout << "CGI Status Header: " << e.what() << std::endl;
		}
	}

	void HttpHeaderCGIStatus::set_value(const std::string &value)
	{
		this->value = value;
		this->parse_value();
	}
}
