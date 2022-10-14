#include <vector>
#include <string>
#include <iostream>
#include "../../../srcs/http/http_header_parser.hpp"
#include "../../../srcs/http/http_header_map.hpp"
#include "../../../srcs/http/headers/http_headers.hpp"

int main()
{
	{
		std::vector<std::string> header_block;
		header_block.push_back("transfer-encoding: chunked");
		
		ws::HttpHeaderParser header_parser(header_block);
		ws::HttpHeaderMap headers = header_parser.parse_block();
	
	// TEST 0
		if (!header_parser.is_valid())
		{
			std::cout << "Failed test: 0" << std::endl;
			return 1;
		}
	
	// TEST 1
		if (headers.begin()->first != "transfer-encoding" || headers.begin()->second->value != "chunked")
		{
			std::cout << "Failed test: 1" << std::endl;
			return 1;
		}

	// TEST 2
		ws::HttpHeaderTransferEncoding *transfer_encoding = static_cast<ws::HttpHeaderTransferEncoding*>(headers.begin()->second);
		if (transfer_encoding->codings.size() != 1 || transfer_encoding->codings[0] != "chunked")
		{
			std::cout << "Failed test: 2" << std::endl;
			return 1;
		}
	}

	// TEST 3
	{
		ws::HttpHeaderTransferEncoding transfer_encoding;
		transfer_encoding.set_value("ChunKed");
		if (transfer_encoding.is_valid == false
			|| transfer_encoding.codings.size() != 1
			|| transfer_encoding.codings[0] != "chunked")
		{
			std::cout << "Failed test: 3" << std::endl;
			return 1;
		}
	}
	
	// TEST 4
	{
		ws::HttpHeaderTransferEncoding transfer_encoding;
		transfer_encoding.set_value("chunked, algo, otro");
		if (transfer_encoding.is_valid == false
			|| transfer_encoding.codings.size() != 3
			|| transfer_encoding.codings[0] != "chunked"
			|| transfer_encoding.codings[1] != "algo"
			|| transfer_encoding.codings[2] != "otro")
		{
			std::cout << "Failed test: 4" << std::endl;
			return 1;
		}
	}

	// TEST 5
	{
		ws::HttpHeaderTransferEncoding transfer_encoding;
		transfer_encoding.set_value("  ,\t    chunked, algo,, otro  ,");
		if (transfer_encoding.is_valid == false
			|| transfer_encoding.codings.size() != 3
			|| transfer_encoding.codings[0] != "chunked"
			|| transfer_encoding.codings[1] != "algo"
			|| transfer_encoding.codings[2] != "otro")
		{
			std::cout << "Failed test: 5" << std::endl;
			return 1;
		}
	}

	// TEST 6
	{
		ws::HttpHeaderTransferEncoding transfer_encoding;
		transfer_encoding.set_value("  , ,,,  ,   ,");
		if (transfer_encoding.is_valid ==true) 
		{
			std::cout << "Failed test: 6" << std::endl;
			return 1;
		}
	}
    return 0;
}
	
