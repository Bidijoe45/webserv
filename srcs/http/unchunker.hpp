#pragma once

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

			Unchunker();
			Stage get_stage() const;
			std::string unchunk();

		private:
			Stage stage_;
			std::string raw_body_;
			std::string unchunked_body_;
	};
}
