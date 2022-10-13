#pragma once

#include <sys/socket.h>
#include <memory>
#include <poll.h>
#include <string>

#include "../http/http_parser.hpp"
#include "data_buffer.hpp"
#include "../settings/server_settings.hpp"

namespace ws {

struct Connection {
	Connection();
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
	ServerSettings settings;
	bool settings_set;
	bool must_close;
};

} //namespace ws
