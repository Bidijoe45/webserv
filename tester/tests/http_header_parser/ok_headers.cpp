#include <vector>
#include <string>
#include <iostream>
#include "../../../srcs/http/http_header_parser.hpp"
#include "../../../srcs/http/http_header_map.hpp"

int main()
{
	std::vector<std::string> header_block;
	header_block.push_back("host: webserv");
	header_block.push_back("content-length:10");
	header_block.push_back("caca:   hola          aaaa");
	header_block.push_back("transfEr-encoDing: chunked");
	header_block.push_back("transfer-encoding: repe");
	header_block.push_back("ContEnT-TYPE: \t\t   valido");
	header_block.push_back("desconocido: eñe");
	header_block.push_back("unknown:");
	header_block.push_back("unknown1");
	header_block.push_back("unknown2: ");
	header_block.push_back("unknown3:      ");
	
	ws::HttpHeaderParser header_parser(header_block);
	ws::HttpHeaderMap headers = header_parser.parse_block();

	// TEST IS VALID
 	if (!header_parser.is_valid())
	{
		std::cout << "Failed test: not valid" << std::endl;
		return 1;
	}
	// TEST SIZE 
	if (headers.size() != 10)
	{
		std::cout << "Failed test: size" << std::endl;
		return 1;
	}

	// TEST HEADER CONTENT
	std::vector<std::string> header_names;
	std::vector<std::string> header_values;

	header_names.push_back("caca");
	header_names.push_back("content-length");
	header_names.push_back("content-type");
	header_names.push_back("desconocido");
	header_names.push_back("host");
	header_names.push_back("transfer-encoding");
	header_names.push_back("unknown");
	header_names.push_back("unknown1");
	header_names.push_back("unknown2");
	header_names.push_back("unknown3");

	header_values.push_back("hola          aaaa");
	header_values.push_back("10");
	header_values.push_back("valido");
	header_values.push_back("eñe");
	header_values.push_back("webserv");
	header_values.push_back("chunked, repe");
	header_values.push_back("");
	header_values.push_back("");
	header_values.push_back("");
	header_values.push_back("");

	ws::HttpHeaderMap::const_iterator map_it = headers.begin();
	ws::HttpHeaderMap::const_iterator map_ite = headers.end();
	std::vector<std::string>::const_iterator names_it = header_names.begin();
	std::vector<std::string>::const_iterator names_ite = header_names.end();
	std::vector<std::string>::const_iterator values_it = header_values.begin();
	std::vector<std::string>::const_iterator values_ite = header_values.end();

	while (map_it != map_ite && names_it != names_ite && values_it != values_ite)
	{
		if (*names_it != map_it->first || *values_it != map_it->second->value)
		{
			std::cout << "Failed test: header content" << std::endl;
			std::cout << "Header name: should be=>|" << *names_it << "|, is=>|" << map_it->first << "|" << std::endl;
			std::cout << "Header value: should be=>|" << *values_it << "|, is=>|" << map_it->second->value << "|" << std::endl;
			return 1;
		}
		map_it++;
		names_it++;
		values_it++;
	}

    return 0;
}
	
