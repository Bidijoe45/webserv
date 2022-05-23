#pragma once

#include <sys/socket.h>
#include <poll.h>

namespace ws {

struct Connection {
	int socket;
	socklen_t addr_len;
	struct sockaddr_storage addr;

	int send_data(char *buff, size_t buff_size);
	int recv_data(char *buff, size_t buff_size);
};

} //namespace ws
