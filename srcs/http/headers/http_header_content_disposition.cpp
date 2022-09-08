#include <string>
#include <iostream>

#include "http_headers.hpp"
#include "../../utils/string_utils.hpp"

namespace ws
{
	HttpHeaderContentDisposition::HttpHeaderContentDisposition()
	{
		this->type = HTTP_HEADER_CONTENT_LENGTH;
	}

	HttpHeaderContentDisposition::~HttpHeaderContentDisposition() {}
	
	HttpHeaderContentDisposition::HttpHeaderContentDisposition(const HttpHeaderContentDisposition &src)
	{
		this->type = src.type;
		this->value = src.value;
	}

	void HttpHeaderContentDisposition::parse_value()
	{
		std::vector<std::string> splitted_header = string_split(this->value, ";");
		std::vector<std::string>::iterator it = splitted_header.begin();
		std::vector<std::string>::iterator ite = splitted_header.end();
		std::vector<std::string> splitted_param;

		while (it != ite)
		{
			*it = string_trim(*it, " ");
			it++;
		}

		if (splitted_header.size() > 0)
			this->type = splitted_header[0];

		it = splitted_header.begin() + 1;
		while (it != ite)
		{
			splitted_param = string_split(*it, "=");
			if (*splitted_param.begin() == "name")
				this->name = string_trim(*splitted_param.begin(), "\"");
			else if (*splitted_param.begin() == "filename")
				this->filename = string_trim(*splitted_param.begin(), "\"");
			it++;
		}
	}

	void HttpHeaderContentDisposition::set_value(const std::string &value)
	{
		this->value = value;
		this->parse_value();
	}

}
