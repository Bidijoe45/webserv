#pragma once

#include <string>
#include <vector>

#include "./http_request.hpp"
#include "../server/data_buffer.hpp"

namespace ws
{
	class HttpParser {
		public:
			HttpParser(DataBuffer &buff);
			HttpRequest parse();
			bool request_is_valid();

		private:
			std::string get_next_line();
			void parse_first_line();
			void parse_headers();
			void parse_body();
			bool valid_request_;
			DataBuffer &buff_;
			std::string line_;
			HttpRequest request_;
	};
	
} // namespace ws
