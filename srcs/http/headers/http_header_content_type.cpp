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
		this->content_type = src.content_type;
		this->parameters = src.parameters;
	}

	void HttpHeaderContentType::parse_value()
	{
		if (this->value.size() == 0)
			return;

		std::vector<std::string> splitted_header = string_split(this->value, ";");
		std::vector<std::string>::iterator it = splitted_header.begin();
		std::vector<std::string>::iterator ite = splitted_header.end();
		std::vector<std::string> splitted_param;

		if (splitted_header.size() == 0)
			return;

		while (it != ite)
		{
			*it = string_trim(*it, " ");
			it++;
		}

		this->content_type = string_to_lower(splitted_header[0]);

		if (splitted_header.size() > 1)
		{
			it = splitted_header.begin() + 1;
			while (it != ite)
			{
				splitted_param = string_split(*it, "=");
				if (splitted_param.size() > 0)
				{
					std::string key = *splitted_param.begin();
					std::string value = "";
					if (splitted_param.size() > 1)
						value = string_trim(*(splitted_param.end() - 1), "\"");
					this->parameters.insert(std::make_pair(key, value));
				}
				it++;
			}
		}
	}

	void HttpHeaderContentType::set_value(const std::string &value)
	{
		this->value = value;
		this->parse_value();
	}

}
