#pragma once

#include <memory>

namespace ws {

	struct DataBuffer {

		DataBuffer();	
		DataBuffer(const DataBuffer &buff);
		~DataBuffer();

		void append(const char *buff, size_t buff_size);
		char *flush(size_t n);
		void clear();

		char *data;
		size_t size;

		private:
			std::allocator<char> allocator_;
	};

}