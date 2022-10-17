#include <string>
#include <iostream>
#include "../../../srcs/http/unchunker.hpp"

int main()
{
	// TEST 0
	ws::Unchunker unchunker;
	unchunker.unchunk("5\r\nhola");
	if (unchunker.is_valid() == false || unchunker.get_stage() != ws::Unchunker::CHUNK_DATA)
	{
		std::cout << "Failed test 0" << std::endl;
		return 1;
	}

	// TEST 1
	unchunker.reset();
	unchunker.unchunk("4\r\nhola\r\n");
	if (unchunker.is_valid() == false || unchunker.get_stage() != ws::Unchunker::CHUNK_SIZE)
	{
		std::cout << "Failed test 1" << std::endl;
		return 1;
	}

	// TEST 2
	unchunker.reset();
	unchunker.unchunk("4\r\nhola\r\n0\r\n");
	if (unchunker.is_valid() == false || unchunker.get_stage() != ws::Unchunker::TRAILER_SECTION)
	{
		std::cout << "Failed test 2" << std::endl;
		return 1;
	}

	// TEST 3
	unchunker.reset();
	unchunker.unchunk("4\r\nhola\r\n0\r\ntrailer: section\r\nsection: trailer\r\n");
	if (unchunker.is_valid() == false || unchunker.get_stage() != ws::Unchunker::TRAILER_SECTION)
	{
		std::cout << "Failed test 3" << std::endl;
		return 1;
	}

    return 0;
}
