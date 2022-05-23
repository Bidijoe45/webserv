#include <sys/socket.h>

#include "connection.hpp"

namespace ws
{

	int Connection::send_data(char *buff, size_t buff_size)
	{
		int total = 0;
		int bytesleft = buff_size;
		int n;

		while(total < buff_size) {
			n = send(this->socket, buff + total, bytesleft, 0);
			if (n == -1) {
				break;
			}

			total += n;
			bytesleft -= n;
		}

		return (n == -1) ? -1 : total;
	}

	int Connection::recv_data(char *buff, size_t buff_size) {
		return recv(this->socket, buff, buff_size, 0);
	}

} // namespace ws

