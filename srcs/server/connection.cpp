#include <sys/socket.h>
#include <iostream>

#include "connection.hpp"

namespace ws
{

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

		return (read == -1 ? -1 : total_read);
	}

} // namespace ws
