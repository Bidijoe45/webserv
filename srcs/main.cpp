#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include <csignal>

#include "server/server.hpp"
#include "server/connection.hpp"
#include "server/data_buffer.hpp"

#include "utils/string_utils.hpp"

#include "server/file_system.hpp"

void atExit() {
	system("leaks webserv");
}

static void close_handler(int sig, siginfo_t *siginfo, void *context)
{
	ws::Server::running = false;
	printf ("Sending PID: %ld, UID: %ld\n", (long)siginfo->si_pid, (long)siginfo->si_uid);
}

int main() {
	struct sigaction act;
	ws::Server server;

	atexit(&atExit);
	memset (&act, 0, sizeof(act));
	act.sa_sigaction = &close_handler;
	act.sa_flags = SA_SIGINFO;

	if (sigaction(SIGINT, &act, NULL) < 0) {
		perror ("sigaction");
		return 1;
	}

	server.run();

	std::cout << "FIN del programa" << std::endl;
}
