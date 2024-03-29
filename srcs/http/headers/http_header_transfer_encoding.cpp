#include <string>
#include <iostream>

#include "http_headers.hpp"
#include "string_utils.hpp"

namespace ws
{
	HttpHeaderTransferEncoding::HttpHeaderTransferEncoding()
	{
		this->type = HTTP_HEADER_TRANSFER_ENCODING;
	}

	HttpHeaderTransferEncoding::~HttpHeaderTransferEncoding() {}

	HttpHeaderTransferEncoding::HttpHeaderTransferEncoding(const HttpHeaderTransferEncoding &src)
	{
		this->type = src.type;
		this->value = src.value;
		this->codings = src.codings;
	}

	void HttpHeaderTransferEncoding::parse_value(const std::string &val)
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

		this->codings.insert(this->codings.end(), elements.begin(), elements.end());
		if (this->codings.size() == 0)
		{
			this->is_valid = false;
			std::cout << "Transfer Encoding Header: empty" << std::endl;
			return;
		}
	}

	void HttpHeaderTransferEncoding::parse_value()
	{
		this->parse_value(this->value);
	}

	void HttpHeaderTransferEncoding::set_value(const std::string &value)
	{
		this->value = value;
		this->parse_value();
	}

	HttpHeaderTransferEncoding &HttpHeaderTransferEncoding::operator+=(const std::string &rhs)
	{
		this->value.append(", " + rhs);
		this->parse_value(rhs);

      	return *this;
	}
}
