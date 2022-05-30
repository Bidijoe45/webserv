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
		private:
			void parse_first_line();
			std::string next_line();
			DataBuffer &buff_;
			HttpRequest request_;
			size_t buffer_index_;


	};
} // namespace ws
