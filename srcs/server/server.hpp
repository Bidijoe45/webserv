#pragma once

#include <vector>
#include <poll.h>

#include "connection.hpp"

namespace ws
{

		class Server {

		public:
			Server(int port);
			Connection accept_new_connection();
			void run();
			int poll_connections();

		private:
			int listen_on(int port);
			std::vector<Connection> connections_;
			std::vector<struct pollfd> poll_fds_;
			int server_socket_;
			int port_;
	};

} // namespace ws
