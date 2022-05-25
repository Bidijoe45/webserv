#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "server/server.hpp"
#include "server/connection.hpp"

void atExit() {
	system("leaks server");
}

int main() {

	//atexit(&atExit);
	ws::Server server(3000);

	server.run();

}
