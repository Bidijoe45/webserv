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

#include "http/request_resolver/resolver.hpp"
#include "settings/server_settings.hpp"
#include "http/http_response.hpp"
#include "http/http_uri.hpp"
#include "http/http_uri_parser.hpp"
#include "settings/location.hpp"

void atExit() {
	system("leaks webserv");
}

static void close_handler(int sig, siginfo_t *siginfo, void *context)
{
	ws::Server::running = false;
	printf ("Sending PID: %ld, UID: %ld\n", (long)siginfo->si_pid, (long)siginfo->si_uid);
}

// int main(int argc, char **argv, char **env) {
// 	struct sigaction act;
// 	ws::Server server;
//
// 	server.set_env(env);
// 	
// //	atexit(&atExit);
// 	memset (&act, 0, sizeof(act));
// 	act.sa_sigaction = &close_handler;
// 	act.sa_flags = SA_SIGINFO;
//
// 	if (sigaction(SIGINT, &act, NULL) < 0) {
// 		perror ("sigaction");
// 		return 1;
// 	}
//
// 	server.run();
//
// 	std::cout << "FIN del programa" << std::endl;
// }

ws::HttpRequest prepare_request()
{
	ws::HttpRequest request;

	request.request_line.method = ws::HTTP_METHOD_GET;

	request.request_line.uri = ws::HttpUriParser("/hola/culo").parse();

	return request;
}

ws::ServerSettings prepare_settings()
{
	ws::ServerSettings settings;

	ws::Location location1;

	location1.path = "/";

	settings.locations.push_back(location1);

	ws::Location location2;

	location2.path = "/hola";
	location2.methods.push_back(ws::HTTP_METHOD_GET);

	settings.locations.push_back(location2);
	
	return settings;
}

int main()
{
	ws::HttpRequest request = prepare_request();
	ws::ServerSettings settings = prepare_settings();


	ws::Resolver resolver(settings, request);

	ws::HttpResponse *response = resolver.get_response();

	std::cout << response->to_string() << std::endl;

	delete response;
}
