#include <string>
#include <iostream>

#include "http_headers.hpp"
#include "string_utils.hpp"

namespace ws
{
	HttpHeaderConnection::HttpHeaderConnection()
	{
		this->type = HTTP_HEADER_CONNECTION;
	}

	HttpHeaderConnection::~HttpHeaderConnection() {}

	HttpHeaderConnection::HttpHeaderConnection(const HttpHeaderConnection &src)
	{
		this->type = src.type;
		this->value = src.value;
		this->options = src.options;
	}

	void HttpHeaderConnection::parse_value(const std::string &val)
	{
		std::vector<std::string> elements = string_split(val, ",");

		std::vector<std::string>::iterator it = elements.begin();
		std::vector<std::string>::iterator ite = elements.end();
		while (it != ite)
		{
			*it = string_trim(*it, " \t");
			*it = string_to_lower(*it);
			it++;
		}

		std::vector<std::string>::const_iterator empty = find(elements.begin(), elements.end(), "");
		while (empty != elements.end())
		{
			elements.erase(empty);
			empty = find(elements.begin(), elements.end(), "");
		}

		this->options.insert(this->options.end(), elements.begin(), elements.end());
	}

	void HttpHeaderConnection::parse_value()
	{
		this->parse_value(this->value);
	}

	void HttpHeaderConnection::set_value(const std::string &value)
	{
		this->value = value;
		this->parse_value();
	}

	HttpHeaderConnection &HttpHeaderConnection::operator+=(const std::string &rhs)
	{
		this->value.append(", " + rhs);
		this->parse_value(rhs);

      	return *this;
	}
}
