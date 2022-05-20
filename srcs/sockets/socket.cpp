#include <iostream>

#include "socket.hpp"

namespace ws
{
	
	//SOCKET

	TCPSocket::TCPSocket() {
		std::cout << "Socket" << std::endl;
	}

	TCPSocket::~TCPSocket() {
		std::cout << "Socket Destructor" << std::endl;
	}

	//SERVER
	TCPServerSocket::TCPServerSocket() : TCPSocket() {
		std::cout << "ServerSocket" << std::endl;
	}

	//CLIENT
	TCPClientSocket::TCPClientSocket() : TCPSocket() {
		
	}


} // namespace ws


