#include <sys/socket.h>
#include <iostream>

#include "connection.hpp"

namespace ws
{
	ConnectionBuffer::ConnectionBuffer() {
		this->data = NULL;
		this->size = 0;
	}

	ConnectionBuffer::ConnectionBuffer(const ConnectionBuffer &buff) {
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

	ConnectionBuffer::~ConnectionBuffer() {
		this->clear();
	}

	void Connection() {
	}

	void ConnectionBuffer::append(const char *buff, size_t buff_size) {
		size_t new_size = this->size + buff_size;

		std::cout << "buff_size: " << buff_size << std::endl;
		std::cout << "new_size: " << new_size << std::endl;

		char *new_data = this->allocator_.allocate(new_size);	

		for (size_t i=0; i < this->size; i++) {
			new_data[i] = this->data[i];
		}

		size_t buff_i = 0;
		for (size_t i=this->size; i < new_size; i++) {
			new_data[i] = buff[buff_i];
			buff_i++;
		}
		
		if (this->data != NULL)
			this->allocator_.deallocate(this->data, this->size);
		this->size = new_size;
		this->data = new_data;
	}

	void ConnectionBuffer::clear() {
		if (this->data != NULL)
			this->allocator_.deallocate(this->data, this->size);
		this->data = NULL;
		this->size = 0;
	}

	int Connection::send_data()
	{
		int total = 0;
		int bytesleft = this->buff.size;
		int n;

		while(total < this->buff.size) {
			n = send(this->socket, buff.data + total, bytesleft, 0);
			if (n == -1) {
				break;
			}

			total += n;
			bytesleft -= n;
		}

		return (n == -1) ? -1 : total;
	}

	int Connection::recv_data() {

		char buff[1024];
		int total_read = 0;
		size_t buffer_size = 1024;
		int read = buffer_size;

		memset(&buff, 0, buffer_size);

		while (read == buffer_size) {
			std::cout << "read: " << read << std::endl;
			read = recv(this->socket, buff, buffer_size, 0); 
			total_read += read;	
			if (read > 0)
				this->buff.append(buff, read);
		}	

		std::cout << "last read: " << read << std::endl;

		return (read == -1 ? -1 : total_read);
	}

} // namespace ws
