#pragma once

#include <string>

namespace ws {

	struct DataBuffer {

		DataBuffer();	
		DataBuffer(const std::string &str);
		~DataBuffer();

		DataBuffer(const DataBuffer &buff);
		DataBuffer &operator=(const DataBuffer &buff);

		void append(const char *buff, size_t buff_size);
		void append(const std::string &str);
		std::string flush(size_t n);
		void clear();
		size_t find(const std::string &str) const;
		std::string get_next_line(std::string delim = "\r\n");
		size_t size() const;

		std::string data;
	};

}
