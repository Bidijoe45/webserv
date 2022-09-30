#pragma once

#include <sys/socket.h>
#include <memory>
#include <poll.h>
#include <string>

#include "http_parser.hpp"
#include "data_buffer.hpp"

namespace ws {

struct Connection {
	~Connection();
	int send_data();
	int recv_data();
	std::string get_ip_address();

	int socket;
	int port;
	socklen_t addr_len;
	struct sockaddr_storage addr;
	DataBuffer buff;
	HttpParser http_parser;
};

} //namespace ws
