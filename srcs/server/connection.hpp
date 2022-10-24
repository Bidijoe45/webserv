#pragma once

#include <sys/socket.h>
#include <memory>
#include <poll.h>
#include <string>

#include "../http/http_parser.hpp"
#include "data_buffer.hpp"
#include "../settings/server_settings.hpp"

#define RECV_BUFF_SIZE 10024

namespace ws {

struct Connection {
	Connection();
	~Connection();
	ssize_t send_data();
	ssize_t recv_data();
	const std::string get_ip_address() const;

	int socket;
	int port;
	socklen_t addr_len;
	struct sockaddr_storage addr;
	DataBuffer recv_buff;
	DataBuffer send_buff;
	HttpParser http_parser;
	ServerSettings settings;
	bool settings_set;
	bool must_close;
};

} //namespace ws
