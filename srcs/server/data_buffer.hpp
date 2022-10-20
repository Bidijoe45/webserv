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
		size_t find(const std::string &str, size_t pos) const;
		std::string get_next_line(std::string delim = "\r\n");
		size_t size() const;
		size_t max_size() const;

		std::string data;
	};

}
