#include "http_parser.hpp"

#include "../server/connection.hpp"
#include "./http_request.hpp"

namespace ws
{

	HttpParser::HttpParser(DataBuffer &buff) {

	}

	HttpRequest HttpParser::parse() {
		return HttpRequest();
	}

} // namespace ws
