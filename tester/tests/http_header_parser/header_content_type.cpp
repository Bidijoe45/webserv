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
		header_block.push_back("Content-Type: multipart/form-data; boundary=something");
		
		ws::HttpHeaderParser header_parser(header_block);
		ws::HttpHeaderMap headers = header_parser.parse_block();
	
	// TEST 0
		if (!header_parser.is_valid())
		{
			std::cout << "Failed test: 0" << std::endl;
			return 1;
		}
	
	// TEST 1
		if (headers.begin()->first != "content-type" || headers.begin()->second->value != "multipart/form-data; boundary=something")
		{
			std::cout << "Failed test: 1" << std::endl;
			return 1;
		}

	// TEST 2
		ws::HttpHeaderContentType *content_type = static_cast<ws::HttpHeaderContentType*>(headers.begin()->second);
		if (content_type->content_type != "multipart/form-data")
		{
			std::cout << "Failed test: 2" << std::endl;
			return 1;
		}

	// TEST 3
		if (content_type->parameters.size() != 1
			|| content_type->parameters.find("boundary") == content_type->parameters.end()
			|| content_type->parameters.at("boundary") != "something")
		{
			std::cout << "Failed test: 3" << std::endl;
			return 1;
		}
	}

	// TEST 3
	{
		ws::HttpHeaderContentType content_type;
		content_type.set_value("Text/Html");
		if (content_type.is_valid == false
			|| content_type.content_type != "text/html"
			|| content_type.parameters.size() != 0) 
		{
			std::cout << "Failed test: 3" << std::endl;
			return 1;
		}
	}
	
	// TEST 4
	{
		ws::HttpHeaderContentType content_type;
		content_type.set_value("hola");
		if (content_type.is_valid == false
			|| content_type.content_type != "hola"
			|| content_type.parameters.size() != 0) 
		{
			std::cout << "Failed test: 4" << std::endl;
			return 1;
		}
	}

	// TEST 5
	{
		ws::HttpHeaderContentType content_type;
		content_type.set_value("");
		if (content_type.is_valid == false
			|| content_type.content_type != ""
			|| content_type.parameters.size() != 0) 
		{
			std::cout << "Failed test: 5" << std::endl;
			return 1;
		}
	}

	// TEST 6
	{
		ws::HttpHeaderContentType content_type;
		content_type.set_value("text/html  ;  param=\"value\";param2=value2;   ;param3=value3;");
		if (content_type.is_valid == false
			|| content_type.content_type != "text/html"
			|| content_type.parameters.size() != 3)
		{
			std::cout << "Failed test: 6" << std::endl;
			return 1;
		}

	// TEST 7
		std::map<std::string, std::string>::iterator it = content_type.parameters.begin();
		if (it->first != "param" || it->second != "value")
		{
			std::cout << "Failed test: 7" << std::endl;
			return 1;
		}
		it++;
		if (it->first != "param2" || it->second != "value2")
		{
			std::cout << "Failed test: 7" << std::endl;
			return 1;
		}
		it++;
		if (it->first != "param3" || it->second != "value3")
		{
			std::cout << "Failed test: 7" << std::endl;
			return 1;
		}
	}

    return 0;
}
	
