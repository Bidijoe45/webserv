#include "data_buffer.hpp"

namespace ws {

	DataBuffer::DataBuffer() {
	this->data = NULL;
	this->size = 0;
	}

	DataBuffer::DataBuffer(const DataBuffer &buff) {
		this->size = buff.size;

		if (buff.data == NULL) {
			this->data = NULL;
			return;	
		}

		this->data = this->allocator_.allocate(buff.size);
		for (size_t i=0; i < buff.size; i++) {
			this->data[i] = buff.data[i];
		}
	}

	DataBuffer::~DataBuffer() {
		this->clear();
	}

	void DataBuffer::append(const char *buff, size_t buff_size) {
		size_t new_size = this->size + buff_size;
		char *new_data = this->allocator_.allocate(new_size);

		if (this->data != NULL) {
			for (size_t i=0; i < this->size; i++) {
				new_data[i] = this->data[i];
			}
		}
		
		size_t i = this->size; 
		size_t buff_i = 0;
		while (i < new_size) {
			new_data[i] = buff[buff_i];
			buff_i++;
			i++;
		}
		
		if (this->data != NULL)
			this->allocator_.deallocate(this->data, this->size);
		this->size = new_size;
		this->data = new_data;
	}

	void DataBuffer::clear() {
		if (this->data != NULL)
			this->allocator_.deallocate(this->data, this->size);
		this->data = NULL;
		this->size = 0;
	}

	char *DataBuffer::flush(size_t n) {
		char *substring = new char[n];

		for (size_t i=0; i < n && i < this->size; i++) {
			substring[i] = this->data[i];
		}
		
		char new_size = this->size - n;
		char *new_data = this->allocator_.allocate(new_size);

		for (size_t i=0; i < new_size; i++) {
			new_data[i] = this->data[n + i];
		}

		this->allocator_.deallocate(this->data, this->size);
		this->data = new_data;
		this->size = new_size;

		return substring;
	}

}