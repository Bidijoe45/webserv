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

	size_t Connection::send_data()
	{
		size_t total = 0;
		size_t bytesleft = this->buff.size();
		size_t n;

		while (total < this->buff.size())
		{
			n = send(this->socket, &buff.data[total], bytesleft, 0);
			if (n == (size_t)-1)
				break;
			total += n;
			bytesleft -= n;
		}

		return (n == (size_t)-1) ? -1 : total;
	}

	size_t Connection::recv_data()
	{
		char buff[10240];
		size_t total_read = 0;
		size_t buffer_size = 10240;
		size_t read = buffer_size;

		memset(&buff, 0, buffer_size);

		while (read == buffer_size)
		{
			read = recv(this->socket, buff, buffer_size, 0); 
			total_read += read;	
			if (read > 0)
				this->buff.append(buff, read);
		}	

		return (read == (size_t)-1 ? -1 : total_read);
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
