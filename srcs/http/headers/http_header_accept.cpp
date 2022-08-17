#include "http_header.hpp"

namespace ws
{
	HttpHeaderAccept::HttpHeaderAccept()
	{
		this->type = HTTP_HEADER_ACCEPT;
	}

	HttpHeaderAccept::~HttpHeaderAccept() {}

	HttpHeaderAccept::HttpHeaderAccept(const HttpHeaderAccept &src)
	{
		this->type = src.type;
		this->value = src.value;
	}

	void HttpHeaderAccept::parse_value() {/* parsear la lista y meterla en las variables que haga falta */}

	void HttpHeaderAccept::parse_added_value(const std::string &value) {/* parsear un añadido a la lista y appendearlo a las variables que haga falta */}

	void HttpHeaderAccept::set_value(const std::string &value)
	{
		this->value = value;
		this->parse_value();
	}

	HttpHeaderAccept &HttpHeaderAccept::operator+=(const std::string &rhs)
	{
		this->value.append(", " + rhs);
		this->parse_added_value(rhs);

      	return *this;
	}
}