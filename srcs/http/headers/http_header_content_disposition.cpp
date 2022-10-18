#include <string>
#include <iostream>

#include "http_headers.hpp"
#include "../../utils/string_utils.hpp"

namespace ws
{
	HttpHeaderContentDisposition::HttpHeaderContentDisposition()
	{
		this->type = HTTP_HEADER_CONTENT_DISPOSITION;
	}

	HttpHeaderContentDisposition::~HttpHeaderContentDisposition() {}
	
	HttpHeaderContentDisposition::HttpHeaderContentDisposition(const HttpHeaderContentDisposition &src)
	{
		this->type = src.type;
		this->value = src.value;
		this->content_type = src.content_type;
		this->name = src.name;
		this->filename = src.filename;
	}

	void HttpHeaderContentDisposition::parse_value()
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
				if (splitted_param.size() > 1)
				{
					if (string_to_lower(*splitted_param.begin()) == "name")
						this->name = string_trim(*(splitted_param.begin() + 1), "\"");
					else if (string_to_lower(*splitted_param.begin()) == "filename")
						this->filename = string_trim(*(splitted_param.begin() + 1), "\"");
				}
				it++;
			}
		}
	}

	void HttpHeaderContentDisposition::set_value(const std::string &value)
	{
		this->value = value;
		this->parse_value();
	}

}
