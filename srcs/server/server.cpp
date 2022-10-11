#include <stdio.h>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <cstdio>
#include <fstream>

#include <iostream>
#include <cstring>

#include "server.hpp"
#include "connection.hpp"
#include "../http/http_parser.hpp"
#include "../http/http_request.hpp"
#include "../settings/parser/settings_parser.hpp"
#include "server_socket.hpp"
#include "../utils/content_type_map.hpp"
#include "../http/request_resolver/request_resolver.hpp"

namespace ws
{
	bool Server::running = false;

	Server::Server()
	{
		this->content_types_.parse_content_types_file("srcs/utils/content_types_list.txt");
		this->http_message_map_ = HttpMessageMap();
	}

	Server::~Server()
	{
		//TODO: cerrar los sockets del servidor
		//TODO: cerrar los sockets de las conexiones tambien jeje
	}

	Connection Server::accept_new_connection(int socket, int port) {
		Connection new_conn;

		new_conn.addr_len = sizeof(struct sockaddr_storage);
		new_conn.socket = accept(socket, (struct sockaddr *)&new_conn.addr, &new_conn.addr_len);
		new_conn.port = port;
		this->connections_.insert(std::make_pair(new_conn.socket, new_conn));

		return new_conn;
	}

	int Server::listen_on(int port) {
		int yes=1;        
		int rv;
		char port_str[5];
		ServerSocket server_socket;
		struct addrinfo hints, *ai, *p;

		server_socket.port = port;
		sprintf(port_str, "%d", port);
		memset(&hints, 0, sizeof hints);

		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		if ((rv = getaddrinfo(NULL, port_str, &hints, &ai)) != 0) {
			fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
			exit(1);
		}
		
		for(p = ai; p != NULL; p = p->ai_next) {
			server_socket.socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
			if (server_socket.socket < 0) { 
				continue;
			}

			setsockopt(server_socket.socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
			fcntl(server_socket.socket, F_SETFL,O_NONBLOCK);

			if (bind(server_socket.socket, p->ai_addr, p->ai_addrlen) < 0) {
				close(server_socket.socket);
				continue;
			}

			break;
		}

		if (p == NULL) {
			return -1;
		}

		freeaddrinfo(ai);

		if (listen(server_socket.socket, 10) == -1) {
			return -1;
		}

		this->server_sockets_.push_back(server_socket);

		return 0;
	}

	void Server::listen_all()
	{
		std::vector<int>::iterator it = this->ports_.begin();
		std::vector<int>::iterator ite = this->ports_.end();

		for (; it != ite; it++)
			this->listen_on(*it);

		it = this->ports_.begin();
/*		std::cout << "Server listening on:";
		for (; it != ite; it++)
			std::cout << " " << *it;
		std::cout << std::endl;
*/
	}

	std::vector<ServerSocket>::iterator Server::get_server_socket(int socket)
	{
		std::vector<ServerSocket>::iterator it = this->server_sockets_.begin();
		std::vector<ServerSocket>::iterator ite = this->server_sockets_.end();

		for (; it!=ite; it++)
		{
			if ((*it).socket == socket)
				return it;
		}

		return this->server_sockets_.end();
	}

	void Server::set_server_sockets_to_poll()
	{
		std::vector<ServerSocket>::iterator server_sockets_it = this->server_sockets_.begin();
		std::vector<ServerSocket>::iterator server_sockets_ite = this->server_sockets_.end();
		for (; server_sockets_it != server_sockets_ite; server_sockets_it++)
		{
			struct pollfd server_poll;	
			server_poll.fd = (*server_sockets_it).socket;
			server_poll.events = POLLIN;

			this->poll_.push_back(server_poll);
		}
	}

	void Server::poll_connections()
	{
		this->set_server_sockets_to_poll();

		while (running)
		{
			int poll_count = poll(&this->poll_[0], this->poll_.size(), POLL_TIMEOUT);

			if (poll_count == -1)
				std::cout << "Error: polling failed" << std::endl;

			for (int i=0; i < this->poll_.size(); i++)
			{
				if (this->poll_[i].revents & POLLIN)
				{
					std::vector<ServerSocket>::iterator ss_it = this->get_server_socket(this->poll_[i].fd);
					if (ss_it != this->server_sockets_.end())
					{
						Connection new_conn = this->accept_new_connection(this->poll_[i].fd, (*ss_it).port);
						this->add_to_poll(new_conn);

						if (new_conn.socket == -1)
						{
							std::cout << "Connection: cannot accept new client connection" << std::endl;
							continue;
						}

//						std::cout << "New connection" << std::endl;
					}
					else
					{
						Connection &conn = this->connections_[this->poll_[i].fd];
						int bytes_read = conn.recv_data();
//						std::cout << "bytes_read: " << bytes_read << std::endl;

						if (bytes_read <= 0)
						{
//							std::cout << "Connection: socket closed by client" << std::endl;
							delete_connection(conn);
							delete_from_poll(i);
							continue ;
						}

						this->on_new_request(conn);
						if (conn.http_parser.must_close() == true)
						{
							std::cout << "Connection: socket closed by server" << std::endl;
							delete_connection(conn);
							delete_from_poll(i);
							continue ;
						}
					}
				}
			}
		}
	}

	void Server::on_new_request(Connection &connection)
	{
//		std::cout << "-- Raw Message by client --" << std::endl;
//		std::fstream log_file("./log_file");
//		std::cout << connection.buff.data << std::endl;
//		std::cout << "-----------------------" << std::endl;
		
		connection.http_parser.parse(connection.buff);
		connection.buff.clear();

		if (connection.http_parser.get_stage() > HttpParser::HEADERS_BLOCK && connection.settings_set == false)
		{
			connection.settings = this->settings_.resolve_settings_hostname(connection.http_parser.get_request(), connection.port);
			connection.http_parser.set_max_body_size(connection.settings.client_max_body_size);
			connection.settings_set = true;
		}

		if (connection.http_parser.get_stage() == HttpParser::COMPLETED)
		{
			HttpRequest http_request = connection.http_parser.get_request();
	
			RequestResolver request_resolver(connection.settings, http_request, this->env_, connection, this->content_types_, this->http_message_map_);
			HttpResponse response = request_resolver.get_response();
	
			connection.buff.append(response.to_string());
	
			connection.send_data();
			connection.buff.clear();

			connection.http_parser.reset();
		}
	}

	void Server::add_to_poll(Connection new_connection)
	{
		struct pollfd new_poll;
		new_poll.fd = new_connection.socket;
		new_poll.events = POLLIN;

		this->poll_.push_back(new_poll);
	}
	
	void Server::set_env(char **env)
	{
		this->env_.set_from(env);
	}

	void Server::delete_connection(const Connection &connection)
	{
		close(connection.socket);
		this->connections_.erase(connection.socket);
	}

	void Server::delete_from_poll(size_t index)
	{
		this->poll_[index] = this->poll_[this->poll_.size() - 1];
		this->poll_.erase(this->poll_.end() - 1);
	}

	void Server::set_ports_from_settings()
	{
		std::vector<ServerSettings>::iterator it = this->settings_.servers.begin();
		std::vector<ServerSettings>::iterator ite = this->settings_.servers.end();
		
		for(; it != ite; it++)
		{
			this->ports_.push_back((*it).port);
		}
	}

	void Server::run() {
		running = true;

		if (this->content_types_.is_valid() == false)	
		{
			std::cout << "content types list file invalid" << std::endl;
			return;
		}

		SettingsParser settings_parser("./server.conf");

		this->settings_ = settings_parser.parse();

		if (!settings_parser.is_valid()) {
			std::cout << "server config file invalid: " << settings_parser.get_error_msg() << std::endl;
			return;
		}
		this->set_ports_from_settings();
		this->listen_all();

		this->poll_connections();
	}


} // namespace ws



