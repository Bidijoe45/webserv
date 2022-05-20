#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <iostream>
#include <cstring>

#include "server.hpp"

#define PORT "3000"
#define MAX_LISTENING_QUEUE_N 5

namespace ws
{

	size_t Connection::send(char *buff, size_t buff_size) {
		
	}
	
	Server::Server(int port) : port_(port){

	}

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

	void Server::run() {
		this->listen_on(this->port_);

		Connection client_conn = accept_new_connection();

		char buff[1024];
		size_t bytes_sent = client_conn.send(buff, 1024);

	}


} // namespace ws



