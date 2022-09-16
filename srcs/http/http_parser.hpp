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

		private:
			void parse_first_line();
			void parse_headers();
			void parse_body();
			void skipOWS();
			std::string get_header_name();
			std::string get_header_value();

			DataBuffer &buff_;
			HttpRequest request_;

	};
	
} // namespace ws
