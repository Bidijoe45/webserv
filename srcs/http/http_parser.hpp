#pragma once

#include "./http_request.hpp"
#include "../server/connection.hpp"

namespace ws
{
	class HttpParser {
		public:
			HttpParser(DataBuffer &buff);
			HttpRequest parse();
		private:

	};
} // namespace ws
