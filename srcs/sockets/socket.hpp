#pragma once

namespace ws
{
	
	//CONNECTION
	class Connection {
		public:
			void send();
			void recv();
			void poll();
	};

	//SOCKET
	class TCPSocket {
		protected:
			TCPSocket();
			~TCPSocket();
			void bind();
			void listen(int port);
			Connection connect();
			Connection accept();

		private:
			//socket
			//queue_length = 20 default
	};

} // namespace ws
