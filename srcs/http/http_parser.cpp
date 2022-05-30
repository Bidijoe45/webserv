#include "http_parser.hpp"


#include "../server/connection.hpp"
#include "./http_request.hpp"

namespace ws
{

	HttpParser::HttpParser(DataBuffer &buff) : buff_(buff) {
		this->buffer_index_ = 0;
	}

	std::string HttpParser::next_line() {
		char *data = this->buff_.data;
		
	}

	void HttpParser::parse_first_line() {
		
	}

	HttpRequest HttpParser::parse() {
		
		return this->request_;
	}

} // namespace ws
