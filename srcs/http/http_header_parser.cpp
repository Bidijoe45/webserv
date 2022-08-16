#include "http_header_parser.hpp"
#include "http_header.hpp"

namespace ws
{
	HttpHeaderParser::HttpHeaderParser() {}

	HttpHeader *HttpHeaderParser::parse(std::string name, std::string value)
	{
		HttpHeader			*parsed_header;
		HTTP_HEADER_TYPE	header_type = HttpHeader::resolve_header_name(name);

		parsed_header = HttpHeader::alloc_new_header(header_type);
		parsed_header->set_value(value);

		return parsed_header;
	}
}
