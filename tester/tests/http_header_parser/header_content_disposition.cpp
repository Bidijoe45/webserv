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
		header_block.push_back("Content-Disposition: form-data; name=\"fieldName\"; filename=\"filename.jpg\"");
		
		ws::HttpHeaderParser header_parser(header_block);
		ws::HttpHeaderMap headers = header_parser.parse_block();
	
	// TEST 0
		if (!header_parser.is_valid())
		{
			std::cout << "Failed test: 0" << std::endl;
			return 1;
		}
	
	// TEST 1
		if (headers.begin()->first != "content-disposition" || headers.begin()->second->value != "form-data; name=\"fieldName\"; filename=\"filename.jpg\"")
		{
			std::cout << "Failed test: 1" << std::endl;
			return 1;
		}

	// TEST 2
		ws::HttpHeaderContentDisposition *content_disposition = static_cast<ws::HttpHeaderContentDisposition*>(headers.begin()->second);
		if (content_disposition->content_type != "form-data"
			|| content_disposition->name != "fieldName"
			|| content_disposition->filename != "filename.jpg")
		{
			std::cout << "Failed test: 2" << std::endl;
			return 1;
		}
	}

	// TEST 3
	{
		ws::HttpHeaderContentDisposition content_disposition;
		content_disposition.set_value("form-data");
		if (content_disposition.is_valid == false
			|| content_disposition.content_type != "form-data"
			|| content_disposition.name != ""
			|| content_disposition.filename != "")
		{
			std::cout << "Failed test: 3" << std::endl;
			return 1;
		}
	}
	
	// TEST 4
	{
		ws::HttpHeaderContentDisposition content_disposition;
		content_disposition.set_value("");
		if (content_disposition.is_valid == false
			|| content_disposition.content_type != ""
			|| content_disposition.name != ""
			|| content_disposition.filename != "")
		{
			std::cout << "Failed test: 4" << std::endl;
			return 1;
		}
	}

	// TEST 5
	{
		ws::HttpHeaderContentDisposition content_disposition;
		content_disposition.set_value("caca; name=  ;;");
		if (content_disposition.is_valid == false
			|| content_disposition.content_type != "caca"
			|| content_disposition.name != ""
			|| content_disposition.filename != "")
		{
			std::cout << "Failed test: 5" << std::endl;
			return 1;
		}
	}

    return 0;
}
	
