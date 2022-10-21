#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <arpa/inet.h>

#include "connection.hpp"

namespace ws
{
	Connection::Connection() : socket(0), port(0), settings_set(false), must_close(false) {}

	Connection::~Connection() {}

	ssize_t Connection::send_data()
	{
		ssize_t sent;

		sent = send(this->socket, buff.data.c_str(), this->buff.size(), 0);
		if (sent != -1)
			this->buff.flush(sent);

		return sent;
	}

	ssize_t Connection::recv_data()
	{
		char buff[RECV_BUFF_SIZE];
		ssize_t read = 0;

		memset(&buff, 0, RECV_BUFF_SIZE);
		read = recv(this->socket, buff, RECV_BUFF_SIZE, 0);
		if (read > 0)
			this->buff.append(buff, read);

		return read;
	}

	std::string Connection::get_ip_address()
	{
		std::string ip_str;

		if (this->addr.ss_family == AF_INET)
		{
			struct sockaddr_in *sa = reinterpret_cast<struct sockaddr_in *>(&this->addr);
			char ip[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &(sa->sin_addr), ip, INET_ADDRSTRLEN);
			ip_str = ip;
		}
		else if (this->addr.ss_family == AF_INET6)
		{
			struct sockaddr_in6 *sa6 = reinterpret_cast<struct sockaddr_in6 *>(&this->addr);
			char ip[INET6_ADDRSTRLEN];
			inet_ntop(AF_INET6, &(sa6->sin6_addr), ip, INET6_ADDRSTRLEN);
			ip_str = ip;
		}
		
		return ip_str;	
	}

} // namespace ws
