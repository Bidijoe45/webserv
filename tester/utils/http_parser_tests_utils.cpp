#include "http_parser_tests_utils.hpp"

#include <iostream>

#include "../../srcs/http/http_request.hpp"
#include "../../srcs/http/http_header_map.hpp"

namespace ws_tester
{
	std::string resolve_http_method(ws::HTTP_METHOD method)
	{
		std::string resolved_method;
		
		switch (method)
		{
			case ws::HTTP_METHOD_GET:
				resolved_method = "GET";
				break;

			case ws::HTTP_METHOD_POST:
				resolved_method = "POST";
				break;

			case ws::HTTP_METHOD_DELETE:
				resolved_method = "DELETE";
				break;
			
			default:
				resolved_method = "INVALID_METHOD";
				break;
		}

		return resolved_method;
	}

	void print_http_uri(ws::HttpUri uri)
	{
		std::cout << "-- Uri --" << std::endl;
		std::cout << "host: " << uri.host << std::endl;
		std::cout << "-- --" << std::endl;
	}

	void print_headers(ws::HttpHeaderMap headers)
	{
		std::string header_type;
		ws::HttpRequest::headers_iterator it;
		
		for (it = headers.begin(); it != headers.end(); it++)
		{
			header_type = ws::HttpHeader::header_type_to_string(it->second->type);
			std::cout << it->first << " = |" << it->second->value << "| header type: " << header_type << std::endl;
		}
		std::cout << "-- --" << std::endl;
	}

	void print_http_request(const ws::HttpRequest &request)
	{	
		std::cout << "-- HttpRequest --" << std::endl;
		std::cout << "method: " << resolve_http_method(request.request_line.method) << std::endl;
		print_http_uri(request.request_line.uri);
		std::cout << "http_version: " << request.request_line.http_version << std::endl;
		
		std::cout << "-- headers: -- " << std::endl;
		print_headers(request.headers);

		std::cout << "body: --" << std::endl;
		std::cout << "|" << request.body << "|" << std::endl;
		std::cout << "-- --" << std::endl;
	}

	bool compare_requests(ws::HttpRequest r1, ws::HttpRequest r2)
	{
		if (r1.request_line == r2.request_line && r1.headers == r2.headers
			&& r1.body == r2.body)
		{
			return true;
		}

		return false;
	}

} // namespace ws_tester
