#pragma once

#include <map>
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
			void on_new_connection(Connection connection);
			void add_to_poll(Connection new_connection);
			void delete_from_poll(size_t index, Connection connection);

		private:
			int listen_on(int port);
			std::map<int, Connection> connections_;
			int server_socket_;
			int port_;
			std::vector<struct pollfd> poll_;
	};

} // namespace ws
