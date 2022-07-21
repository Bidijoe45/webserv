#include "http_parser_tests_utils.hpp"

#include <iostream>

#include "../../../srcs/http/http_request.hpp"

namespace ws_tester
{
	std::string resolve_http_method(ws::HTTP_METHOD method)
	{
		std::string resolved_method;
		
		switch (method)
		{
			case ws::GET:
				resolved_method = "GET";
				break;

			case ws::POST:
				resolved_method = "POST";
				break;

			case ws::DELETE:
				resolved_method = "DELETE";
				break;
			
			default:
				resolved_method = "INVALID_METHOD";
				break;
		}

		return resolved_method;
	}

	void print_http_request(ws::HttpRequest request)
	{	
		std::cout << "-- HttpRequest --" << std::endl;
		std::cout << "method: " << resolve_http_method(request.method) << std::endl;
		std::cout << "url: " << request.url << std::endl;
		std::cout << "http_version: " << request.http_version << std::endl;
		
		std::cout << "headers: -- " << std::endl;
		ws::HttpRequest::headers_iterator headers_it = request.headers.begin();
		for (; headers_it != request.headers.end(); headers_it++)
			std::cout << (*headers_it).first << " = " << (*headers_it).second << std::endl;
		std::cout << "-- --" << std::endl;

		std::cout << "body: --" << std::endl;
		std::cout << "|" << request.body << "|" << std::endl;
		std::cout << "-- --" << std::endl;
	}

	bool compare_requests(ws::HttpRequest r1, ws::HttpRequest r2)
	{
		if (r1.method == r2.method && r1.url == r2.url && r1.headers == r2.headers
			&& r1.body == r2.body)
		{
			return true;
		}

		return false;
	}

} // namespace ws_tester
