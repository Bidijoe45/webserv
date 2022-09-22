#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "connection.hpp"

namespace ws
{

	Connection::~Connection() {}

	int Connection::send_data()
	{
		int total = 0;
		int bytesleft = this->buff.size();
		int n;

		while(total < this->buff.size())
		{
			n = send(this->socket, &buff.data[total], bytesleft, 0);
			if (n == -1)
				break;
			total += n;
			bytesleft -= n;
		}

		return (n == -1) ? -1 : total;
	}

	int Connection::recv_data()
	{
		char buff[1024];
		int total_read = 0;
		size_t buffer_size = 1024;
		int read = buffer_size;

		memset(&buff, 0, buffer_size);

		while (read == buffer_size)
		{
			std::cout << "read: " << read << std::endl;
			read = recv(this->socket, buff, buffer_size, 0); 
			total_read += read;	
			if (read > 0)
				this->buff.append(buff, read);
		}	

		return (read == -1 ? -1 : total_read);
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
