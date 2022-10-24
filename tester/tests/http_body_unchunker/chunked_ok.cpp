#include <string>
#include "../../../srcs/http/unchunker.hpp"
#include <iostream>

int main()
{
	// TEST 0
	ws::Unchunker unchunker;
	unchunker.unchunk("D\r\nHola que tal?\r\n7\r\nYo bien\r\n5\r\nAdios\r\n0\r\n\r\n");
	std::string body = unchunker.get_unchunked_body();
	std::string body_model = "Hola que tal?Yo bienAdios";
	if (unchunker.is_valid() == false || body != body_model || unchunker.get_stage() != ws::Unchunker::COMPLETED)
	{
		std::cout << "Failed test 0" << std::endl;
		return 1;
	}

	// TEST 1: including LF and CR in the body
	unchunker.reset();
	unchunker.unchunk("e\r\nHola que tal?\n\r\n9\r\nYo bien, \r\n7\r\nAdios\r\n\r\n0\r\n\r\n");
	body = unchunker.get_unchunked_body();
	body_model = "Hola que tal?\nYo bien, Adios\r\n";
	if (unchunker.is_valid() == false || body != body_model || unchunker.get_stage() != ws::Unchunker::COMPLETED)
	{
		std::cout << "Failed test 1" << std::endl;
		return 1;
	}

	// TEST 2: chunk extensions (ignored)
	unchunker.reset();
	unchunker.unchunk("E;extension\r\nHola que tal?\n\r\n9\r\nYo bien, \r\n7  ; chunk_ext=extension\r\nAdios\r\n\r\n0\r\n\r\n");
	body = unchunker.get_unchunked_body();
	body_model = "Hola que tal?\nYo bien, Adios\r\n";
	if (unchunker.is_valid() == false || body != body_model || unchunker.get_stage() != ws::Unchunker::COMPLETED)
	{
		std::cout << "Failed test 2" << std::endl;
		return 1;
	}

	// TEST 3: trailer section (ignored)
	unchunker.reset();
	unchunker.unchunk("E\r\nHola que tal?\n\r\n9\r\nYo bien, \r\n7\r\nAdios\r\n\r\n0\r\ntrailer:section\r\ntrailer2: section\r\nignoring: trailer section\r\n\r\n");
	body = unchunker.get_unchunked_body();
	body_model = "Hola que tal?\nYo bien, Adios\r\n";
	if (unchunker.is_valid() == false || body != body_model || unchunker.get_stage() != ws::Unchunker::COMPLETED)
	{
		std::cout << "Failed test 3" << std::endl;
		return 1;
	}

    return 0;
}
