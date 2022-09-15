#pragma once

#include <string>
#include <vector>

#include "http_request.hpp"
#include "../server/data_buffer.hpp"

namespace ws
{
	class HttpParser {
		public:
			HttpParser(DataBuffer &buff);
			HttpRequest parse();
			bool request_is_valid();

		private:
			void parse_first_line();
			void parse_headers();
			void parse_body();
			void skipOWS();
			std::string get_header_name();
			std::string get_header_value();

			bool valid_request_;
			DataBuffer &buff_;
			HttpRequest request_;

	};
	
} // namespace ws
