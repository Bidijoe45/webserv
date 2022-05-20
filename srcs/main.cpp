
#include "server/server.hpp"

int main() {

	ws::Server server(3000);

	server.run();

}
