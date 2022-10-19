#pragma once

#include <map>
#include <vector>
#include <poll.h>

#include "connection.hpp"
#include "../settings/settings.hpp"
#include "server_socket.hpp"
#include "../utils/content_type_map.hpp"
#include "../utils/env_map.hpp"
#include "../utils/http_message_map.hpp"

#define POLL_TIMEOUT -1

namespace ws
{

	class Server {

		public:
			typedef std::map<int, Connection>::iterator connections_iterator;
			typedef std::vector<int>::iterator server_sockets_iterator;

			Server();
			~Server();
			Connection accept_new_connection(int socket, int port);
			void run();
			void poll_connections();
			void on_new_request(Connection &connection);
			void add_to_poll(Connection new_connection);
			void set_env(char **env);

			static bool running;

		private:
			int listen_on(int port);
			void listen_all();
			void set_server_sockets_to_poll();
			void delete_connection(const Connection &connection);
			void delete_from_poll(size_t index);
			std::vector<ServerSocket>::iterator get_server_socket(int socket);
			void set_ports_from_settings();
			void parse_request(Connection &connection);
			bool check_connection_header(const HttpRequest &http_request);
			
			std::map<int, Connection> connections_;
			std::vector<ServerSocket> server_sockets_;
			std::vector<int> ports_;
			std::vector<struct pollfd> poll_;
			Settings settings_;
			EnvMap env_; 
			ContentTypeMap content_types_;
			HttpMessageMap http_message_map_;

	};

} // namespace ws
