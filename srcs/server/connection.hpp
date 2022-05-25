#pragma once

#include <sys/socket.h>
#include <memory>
#include <poll.h>

namespace ws {

struct ConnectionBuffer {

	ConnectionBuffer();	
	ConnectionBuffer(const ConnectionBuffer &buff);
	~ConnectionBuffer();

	void append(const char *buff, size_t buff_size);
	void clear();

	char *data;
	size_t size;

	private:
		std::allocator<char> allocator_;
};

struct Connection {
	int socket;
	socklen_t addr_len;
	struct sockaddr_storage addr;
	ConnectionBuffer buff;	

	int send_data();
	int recv_data();
};

} //namespace ws