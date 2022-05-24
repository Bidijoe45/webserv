#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

#include <iostream>
#include <cstring>

#include "server.hpp"

#define PORT "3000"
#define MAX_LISTENING_QUEUE_N 5

namespace ws
{

	Server::Server(int port) : port_(port) { }

	Connection Server::accept_new_connection() {
		Connection new_conn;

		new_conn.addr_len = sizeof(struct sockaddr_storage);
		new_conn.socket = accept(this->server_socket_,  (struct sockaddr *)&new_conn.addr, &new_conn.addr_len);

		return new_conn;
	}

	int Server::listen_on(int port) {
		int yes=1;        
		int rv;

		struct addrinfo hints, *ai, *p;

		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
			fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
			exit(1);
		}
		
		for(p = ai; p != NULL; p = p->ai_next) {
			this->server_socket_ = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
			if (this->server_socket_ < 0) { 
				continue;
			}
			
			setsockopt(this->server_socket_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
			fcntl(this->server_socket_, F_SETFL,O_NONBLOCK);

			if (bind(this->server_socket_, p->ai_addr, p->ai_addrlen) < 0) {
				close(this->server_socket_);
				continue;
			}

			break;
		}

		if (p == NULL) {
			return -1;
		}

		freeaddrinfo(ai);

		// Listen
		if (listen(this->server_socket_, 10) == -1) {
			return -1;
		}

		return 0;
	}

	void Server::on_new_connection(Connection connection) {
		
	}

	int Server::poll_connections() {

		struct pollfd server_poll;	
		server_poll.fd = this->server_socket_;
		server_poll.events = POLLIN;
		char buff[1024];

		this->poll_.push_back(server_poll);

		for (;;) {
			int poll_count = poll(&this->poll_[0], this->poll_.size(), -1);

			if (poll_count == -1) {
				std::cout << "Error: polling failed" << std::endl;
			}

			for (int i=0; i < this->poll_.size(); i++) {
				
				memset(buff, 0, 1024);

				if (this->poll_[i].revents & POLLIN) {

					if (this->poll_[i].fd == this->server_socket_) {
						Connection new_conn = this->accept_new_connection();
						this->add_to_poll(new_conn);

						if (new_conn.socket == -1) {
							std::cout << "Connection: cannot accept new client connection" << std::endl;
							continue;
						}

						std::cout << "Connection: new connection" << std::endl;
					}
					else {
						Connection conn = this->connections_[this->poll_[i].fd];
						int bytes_read = conn.recv_data(buff, 1024);
						std::cout << "Bytes read: " << bytes_read << std::endl;
						if (bytes_read <= 0) {
							std::cout << "Connection: socket closed by client" << std::endl;
							delete_from_poll(i, conn);
							continue ;
						}

						std::cout << "-- Message by client --" << std::endl;
						std::cout << buff << std::endl;
						std::cout << "-----------------------" << std::endl;

						char buff_tmp[4];
						memcpy(buff_tmp, "Msg!", 4);
						conn.send_data(buff_tmp, 4);
					}

				}

			}
		}
	}

	void Server::add_to_poll(Connection new_connection)
	{
		struct pollfd new_poll;
		new_poll.fd = new_connection.socket;
		new_poll.events = POLLIN;

		this->poll_.push_back(new_poll);
		this->connections_.insert(std::make_pair(new_connection.socket, new_connection));
	}

	void Server::delete_from_poll(size_t index, Connection connection)
	{
		this->connections_.erase(connection.socket);
		this->poll_[index] = this->poll_[this->poll_.size() - 1];
		this->poll_.erase(this->poll_.end() - 1);
	}

	void Server::run() {
		this->listen_on(this->port_);
		this->poll_connections();
	}


} // namespace ws



