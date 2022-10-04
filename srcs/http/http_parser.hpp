#pragma once

#include <string>
#include <vector>

#include "http_request.hpp"
#include "../server/data_buffer.hpp"
#include "unchunker.hpp"

namespace ws
{
	class HttpParser
	{
		public:
			enum Stage
			{
				REQUEST_LINE,
				HEADERS_BLOCK,
				SIMPLE_BODY,
				CHUNKED_BODY,
				COMPLETED
			};

			HttpParser();
			void parse(const DataBuffer &new_buff);
			HttpRequest get_request() const;
			Stage get_stage() const;
			void reset();
			bool must_close() const;

		private:
			void parse_first_line();
			void parse_headers();
			void parse_body();
			void parse_chunked_body();
			void skipOWS();
			std::string get_header_name();
			std::string get_header_value();

			DataBuffer buff_;
			size_t buff_pos_;
			HttpRequest request_;
			Stage stage_;
			size_t expected_body_size_;
			std::vector<std::string> transfer_codings_;
			Unchunker unchunker_;
			bool must_close_;
	};
	
} // namespace ws
