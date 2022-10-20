#include <string>

#include "data_buffer.hpp"

namespace ws {

	DataBuffer::DataBuffer() {}

	DataBuffer::DataBuffer(const std::string &str) : data(str) {}

	DataBuffer::DataBuffer(const DataBuffer &buff) : data(buff.data) {}

	DataBuffer &DataBuffer::operator=(const DataBuffer &buff)
	{
		if (this == &buff)
			return *this;
		this->data = buff.data;

		return *this;
	}

	DataBuffer::~DataBuffer() {}

	void DataBuffer::append(const char *buff, size_t buff_size)
	{
		this->data.append(buff, buff_size);
	}

	void DataBuffer::append(const std::string &str)
	{
		this->data.append(str);
	}

	void DataBuffer::clear()
	{
		this->data.clear();
	}

	std::string DataBuffer::flush(size_t n)
	{
		std::string ret = this->data.substr(0, n);
		this->data.erase(0, n);

		return ret;
	}

	size_t DataBuffer::find(const std::string &str, size_t pos = 0) const
	{
		return this->data.find(str, pos);
	}

	std::string DataBuffer::get_next_line(std::string delim)
	{
		size_t delim_pos = this->find(delim);
		std::string line;

		if (delim_pos == std::string::npos)
			return this->flush(this->size());

		line = this->flush(delim_pos);
		this->flush(delim.size());

		return line;
	}

	size_t DataBuffer::size() const
	{
		return this->data.size();
	}

	size_t DataBuffer::max_size() const
	{
		return this->data.max_size();
	}

}
