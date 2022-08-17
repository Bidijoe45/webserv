#include "http_header_parser.hpp"
#include "http_header.hpp"

namespace ws
{
	HttpHeaderParser::HttpHeaderParser() {}

	HttpHeader *HttpHeaderParser::parse(const std::string &name, const std::string &value)
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
}
