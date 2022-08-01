#pragma once

#include <string>
#include <vector>

#include "http_request.hpp"
#include "data_buffer.hpp"

namespace ws
{
	class HttpParser {
		public:
			HttpParser(DataBuffer &buff);
			HttpRequest parse();
			bool request_is_valid();

		private:
			std::string get_next_line();
			void parse_method();
			void parse_uri();
			void parse_version();
			void check_space();
			void parse_first_line();
			void parse_headers();
			void parse_body();
			void advance(size_t n);
			void skipOWS();
			std::string get_header_name();
			std::string get_header_value();

			bool valid_request_;
			DataBuffer &buff_;
			size_t line_pos_;
			std::string line_;
			HttpRequest request_;

	};
	
} // namespace ws
