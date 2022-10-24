#include <string>
#include <iostream>
#include "../../../srcs/http/unchunker.hpp"

int main()
{
	// TEST 0
	ws::Unchunker unchunker;
	unchunker.unchunk("0\r\n\r\n");
	std::string body = unchunker.get_unchunked_body();
	std::string body_model = "";
	if (unchunker.is_valid() == false || body != body_model || unchunker.get_stage() != ws::Unchunker::COMPLETED)
	{
		std::cout << "Failed test 0" << std::endl;
		return 1;
	}

	// TEST 1
	unchunker.reset();
	unchunker.unchunk(""); // incomplete request, continue receiving...
	body = unchunker.get_unchunked_body();
	body_model = "";
	if (unchunker.is_valid() == false || body != body_model || unchunker.get_stage() != ws::Unchunker::CHUNK_SIZE)
	{
		std::cout << "Failed test 1" << std::endl;
		return 1;
	}

    return 0;
}
