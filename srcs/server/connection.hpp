#pragma once

#include <sys/socket.h>
#include <memory>
#include <poll.h>

#include "./data_buffer.hpp"

namespace ws {

struct Connection {
	~Connection();
	int send_data();
	int recv_data();

	int socket;
	socklen_t addr_len;
	struct sockaddr_storage addr;
	DataBuffer buff;	
};

} //namespace ws
