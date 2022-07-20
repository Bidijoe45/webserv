#pragma once

#include <string>

namespace ws {

	struct DataBuffer {

		DataBuffer();	
		DataBuffer(const std::string &str);
		~DataBuffer();

		DataBuffer(const DataBuffer &buff);
		DataBuffer &operator==(const DataBuffer &buff);

		void append(const char *buff, size_t buff_size);
		std::string flush(size_t n);
		void clear();
		size_t find(const std::string &str) const;
		size_t size() const;

		std::string data;
	};

}