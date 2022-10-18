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

std::string prepare_raw_body()
{
    std::string raw_body;

    raw_body = 
    "--boundary\r\n"
    "Content-Type: text/plain\r\n\r\n"
    "file 1 content"
    "--boundary\r\n"
    "Content-Disposition: form-data; name=\"datafile2\"; filename=\"file2.txt\"\r\n"
    "Content-Type: image/gif\r\n\r\n"
    "file 2 content"
    "--boundary\r\n"
    "Content-Disposition: form-data; name=\"datafile3\"; filename=\"file3.gif\"\r\n"
    "Content-Type: image/gif\r\n\r\n"
    "file 3 content"
    "--boundary--\r\n";

    return raw_body;
}

int main()
{
	ws::HttpMultipartBodyParser parser(prepare_raw_body(), "boundary");
	ws::HttpMultipartBody parsed_body = parser.parse();

	std::cout << "Valid: " << parser.is_valid() << std::endl;

	return 0;
}
