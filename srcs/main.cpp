#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include <csignal>
#include <exception>

#include "server/server.hpp"
#include "server/connection.hpp"
#include "server/data_buffer.hpp"
#include "utils/string_utils.hpp"
#include "server/file_system.hpp"

#include "settings/server_settings.hpp"
#include "http/http_response.hpp"
#include "http/http_uri.hpp"
#include "http/http_uri_parser.hpp"
#include "settings/location.hpp"

#include "http/http_multipart_body.hpp"
#include "http/http_multipart_body_parser.hpp"

static void close_handler(int sig, siginfo_t *siginfo, void *context)
{
	ws::Server::running = false;
	(void)sig;
	(void)siginfo;
	(void)context;
}

 int main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;

 	struct sigaction act;
 	ws::Server server;

 	server.set_env(env);
 	
 	memset (&act, 0, sizeof(act));
 	act.sa_sigaction = &close_handler;
 	act.sa_flags = SA_SIGINFO;

 	if (sigaction(SIGINT, &act, NULL) < 0) {
		std::cout << "Error: Sigaction init failed" << std::endl;
 		return 1;
 	}

 	server.run();

 	std::cout << "FIN del programa" << std::endl;

	return 0;
}
