#include "http_header_parser.hpp"
#include "http_header.hpp"
#include "../utils/string_utils.hpp"
#include <iostream>
#include <string>

namespace ws
{
	HttpHeaderParser::HttpHeaderParser(const DataBuffer &buff) : buff_(buff), valid_(true) {}

	bool HttpHeaderParser::is_valid()
	{
		return this->valid_;
	}

	HttpHeader *HttpHeaderParser::parse_header(const std::string &name, const std::string &value)
	{
		HttpHeader			*parsed_header;
		HTTP_HEADER_TYPE	header_type = HttpHeader::resolve_header_name(name);

		parsed_header = HttpHeader::alloc_new_header(header_type);
		parsed_header->set_value(value);

		return parsed_header;
	}

	void HttpHeaderParser::combine_value(HttpHeader *header, const std::string &added_value)
	{
		HttpHeaderListBased *list_based_header;

		list_based_header = dynamic_cast<HttpHeaderListBased *>(header);

		if (!list_based_header)
			throw std::runtime_error("Request: found more than one singleton header with the same name");
		*(HttpHeaderListBased *)header += added_value;
	}

	std::string HttpHeaderParser::get_header_name()
	{
		std::string	header_name;
		size_t		colon_pos = this->current_line_.find_first_of(':', this->line_pos_);

		header_name = this->current_line_.substr(this->line_pos_, colon_pos);
		header_name = string_to_lower(header_name, header_name.size());

		if (!is_token(header_name))
			throw std::runtime_error("Header Parser: invalid header name characters");

		this->advance(colon_pos + 1);
		return header_name;
	}

	std::string	HttpHeaderParser::get_header_value()
	{
		std::string	header_value;
		size_t	value_end = this->current_line_.find_last_not_of(" \t");

		header_value = this->current_line_.substr(this->line_pos_, value_end - this->line_pos_ + 1);
		for (size_t i = 0; header_value[i]; i++)
		{
			if (!std::isprint(header_value[i])
				&& header_value[i] != ' ' && header_value[i] != '\t'
				&& !is_obstext(header_value[i]))
			{
				throw std::runtime_error("Header Parser: invalid header value characters");
			}
		}
		return header_value;
	}

	HttpHeaderMap HttpHeaderParser::parse_block()
	{
		std::string				header_name;
		std::string				header_value;
		std::string  			current_line;
		HttpHeader				*parsed_header;
		HttpHeaderMap           map;
		HttpHeaderMap::iterator	found_header;

		this->current_line_ = this->buff_.get_next_line();

		try
		{
			while (this->current_line_.size() != 0)
			{
				this->line_pos_ = 0;
				header_name = get_header_name();
				this->skipOWS();
				header_value = get_header_value();
				found_header = map.find(header_name);
				if (found_header != map.end())
					this->combine_value(found_header->second, header_value);
				else
				{
					parsed_header = this->parse_header(header_name, header_value);
					map.insert(header_name, parsed_header);
				}
				this->current_line_ = this->buff_.get_next_line();
			}
		}
		catch (const std::runtime_error &e)
		{
			std::cout << e.what() << std::endl;
			this->valid_ = false;
		}
		return map;
	}

	void HttpHeaderParser::advance(size_t n = 1)
	{
	    if ((this->line_pos_ + n) >= this->current_line_.size())
	        throw std::runtime_error("Header Parser: trying to advance past the end of string");
		this->line_pos_ += n;
	}

	void HttpHeaderParser::skipOWS()
	{
		this->line_pos_ = this->current_line_.find_first_not_of(" \t", this->line_pos_);
	}

}
