#pragma once

#include "./http_request.hpp"
#include "../server/connection.hpp"

namespace ws
{
	class HttpParser {
		public:
			HttpParser(ConnectionBuffer &buff);
			HttpRequest parse();
		private:

	};
} // namespace ws
