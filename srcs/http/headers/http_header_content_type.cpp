#include <string>
#include <iostream>

#include "http_headers.hpp"
#include "../../utils/string_utils.hpp"

namespace ws
{
	HttpHeaderContentType::HttpHeaderContentType()
	{
		this->type = HTTP_HEADER_CONTENT_TYPE;
	}

	HttpHeaderContentType::~HttpHeaderContentType() {}
	
	HttpHeaderContentType::HttpHeaderContentType(const HttpHeaderContentType &src)
	{
		this->type = src.type;
		this->value = src.value;
	}

	void HttpHeaderContentType::parse_value()
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
			std::string key = *splitted_param.begin();
			std::string value = string_trim(*(splitted_param.end() - 1), "\"");
			this->parameters.insert(std::make_pair(key, value));
			it++;
		}

	}

	void HttpHeaderContentType::set_value(const std::string &value)
	{
		this->value = value;
		this->parse_value();
	}

}
