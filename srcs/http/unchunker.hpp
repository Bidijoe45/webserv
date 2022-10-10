#pragma once

#include <string>
#include "../server/data_buffer.hpp"

namespace ws
{
	class Unchunker
	{
		public:
			enum Stage
			{
				CHUNK_SIZE,
				CHUNK_DATA,
				TRAILER_SECTION,
				COMPLETED
			};
			enum Status
			{
				INCOMPLETE,
				OK
			};

			Unchunker();
			void unchunk(const std::string &raw_body);
			Stage get_stage() const;
			std::string get_unchunked_body() const;
			bool is_valid() const;
			void reset();

		private:
			void parse_size();
			void parse_chunk_data();
			void parse_trailer_section();

			Stage stage_;
			DataBuffer raw_body_buff_;
			std::string unchunked_body_;
			size_t pos_;
			size_t chunk_size_;
			std::string chunk_data_;
			bool valid_;
			Status status_;
	};
}
