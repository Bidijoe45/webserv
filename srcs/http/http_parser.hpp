#pragma once

#include <string>
#include <vector>

#include "./http_request.hpp"
#include "../server/connection.hpp"

namespace ws
{
	class HttpParser {
		public:
			HttpParser(DataBuffer &buff);
			HttpRequest parse();
			bool request_is_valid();

		private:
			std::string next_line();
			void parse_first_line();
			void parse_headers();
			void parse_body();
			std::vector<std::string> split_line(std::string line, char delimiter);
			HTTP_METHOD resolve_request_method(std::string str);
			std::string resolve_request_url(std::string str);

			std::string raw_request_;
			HttpRequest request_;
			bool valid_request_;
	};
} // namespace ws
