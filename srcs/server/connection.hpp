#pragma once

#include <sys/socket.h>
#include <memory>
#include <poll.h>

#include "./data_buffer.hpp"

namespace ws {

struct Connection {
	int socket;
	socklen_t addr_len;
	struct sockaddr_storage addr;
	DataBuffer buff;	

	int send_data();
	int recv_data();
};

} //namespace ws