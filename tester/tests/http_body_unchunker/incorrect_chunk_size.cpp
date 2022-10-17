#include <string>
#include <iostream>
#include "../../../srcs/http/unchunker.hpp"

int main()
{
	// TEST 0
	ws::Unchunker unchunker;
	unchunker.unchunk("5\r\nholaaaaaa\r\n0\r\n\r\n");
	if (unchunker.is_valid() == true)
	{
		std::cout << "Failed test 0" << std::endl;
		return 1;
	}

	// TEST 1
	unchunker.reset();
	unchunker.unchunk("5\r\nhola\r\n0\r\n\r\n");
	if (unchunker.is_valid() == true)
	{
		std::cout << "Failed test 1" << std::endl;
		return 1;
	}

	// TEST 2
	unchunker.reset();
	unchunker.unchunk("7\r\nhola\r\n0\r\n\r\n");
	if (unchunker.is_valid() == true)
	{
		std::cout << "Failed test 2" << std::endl;
		return 1;
	}

	// TEST 3
	unchunker.reset();
	unchunker.unchunk("9\r\nhola\r\n0\r\n\r\n");
	std::string body = unchunker.get_unchunked_body();
	std::string body_model = "hola\r\n0\r\n";
	if (unchunker.is_valid() == false || body != body_model || unchunker.get_stage() != ws::Unchunker::CHUNK_SIZE)
	{
		std::cout << "Failed test 3" << std::endl;
		return 1;
	}

    return 0;
}
