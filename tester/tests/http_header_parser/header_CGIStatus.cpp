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
		header_block.push_back("Status: 501 Not Implemented");
		
		ws::HttpHeaderParser header_parser(header_block);
		ws::HttpHeaderMap headers = header_parser.parse_block();
	
	// TEST 0
		if (!header_parser.is_valid())
		{
			std::cout << "Failed test: 0" << std::endl;
			return 1;
		}
	
	// TEST 1
		if (headers.begin()->first != "status" || headers.begin()->second->value != "501 Not Implemented")
		{
			std::cout << "Failed test: 1" << std::endl;
			return 1;
		}

	// TEST 2
		ws::HttpHeaderCGIStatus *status = static_cast<ws::HttpHeaderCGIStatus*>(headers.begin()->second);
		if (status->status_code != 501 || status->reason_phrase != "Not Implemented")
		{
			std::cout << "Failed test: 2" << std::endl;
			return 1;
		}
	}

	// TEST 3
	{
		ws::HttpHeaderCGIStatus status;
		status.set_value("200");
		if (status.is_valid == false
			|| status.status_code != 200
			|| status.reason_phrase.size() > 0)
		{
			std::cout << "Failed test: 3" << std::endl;
			return 1;
		}
	}
	
	// TEST 4
	{
		ws::HttpHeaderCGIStatus status;
		status.set_value("200 OK");
		if (status.is_valid == false
			|| status.status_code != 200
			|| status.reason_phrase != "OK")
		{
			std::cout << "Failed test: 4" << std::endl;
			return 1;
		}
	}

	// TEST 5
	{
		ws::HttpHeaderCGIStatus status;
		status.set_value("2000 OK");
		if (status.is_valid == true) 
		{
			std::cout << "Failed test: 5" << std::endl;
			return 1;
		}
	}

	// TEST 6
	{
		ws::HttpHeaderCGIStatus status;
		status.set_value(" 200 OK");
		if (status.is_valid == true)
		{
			std::cout << "Failed test: 6" << std::endl;
			return 1;
		}
	}

	// TEST 7
	{
		ws::HttpHeaderCGIStatus status;
		status.set_value("2x0 OK");
		if (status.is_valid == true)
		{
			std::cout << "Failed test: 7" << std::endl;
			return 1;
		}
	}

    return 0;
}
	
