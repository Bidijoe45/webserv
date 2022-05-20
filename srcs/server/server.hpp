
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

namespace ws
{

	struct Connection {
		int socket;
		socklen_t addr_len;
		struct sockaddr_storage addr;

		size_t send(char *buff, size_t buff_size);
		void recv();
	};

	class Server {

		public:
			Server(int port);
			Connection accept_new_connection();
			void run();

		private:
			int listen_on(int port);

			int server_socket_;
			int port_;
	};

} // namespace ws
