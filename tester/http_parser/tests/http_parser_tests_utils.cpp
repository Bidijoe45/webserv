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

	std::string resolve_http_header_type(ws::HTTP_HEADER_TYPE header_type)
	{
		std::string resolved_header_type;

		switch (header_type)
		{
			case ws::HTTP_HEADER_UNKNOWN:
				resolved_header_type = "HTTP_HEADER_UNKNOWN";
				break;

			case ws::HTTP_HEADER_HOST:
				resolved_header_type = "HTTP_HEADER_HOST";
				break;

			case ws::HTTP_HEADER_ACCEPT:
				resolved_header_type = "HTTP_HEADER_ACCEPT";
				break;
			
			default:
				resolved_header_type = "WTF";
				break;
		}

		return resolved_header_type;
	}

	void print_http_uri(ws::HttpUri uri)
	{
		std::cout << "-- Uri --" << std::endl;
		std::cout << "host: " << uri.host << std::endl;
		std::cout << "-- --" << std::endl;
	}

	void print_headers(std::map<std::string, ws::HttpHeader*> headers)
	{
		std::string header_type;
		ws::HttpRequest::headers_iterator headers_it = headers.begin();

		for (; headers_it != headers.end(); headers_it++)
		{
			header_type = resolve_http_header_type((*headers_it).second->type);
			std::cout << (*headers_it).first << " = " << header_type << std::endl;
		}
		std::cout << "-- --" << std::endl;
	}

	void print_http_request(ws::HttpRequest request)
	{	
		std::cout << "-- HttpRequest --" << std::endl;
		std::cout << "method: " << resolve_http_method(request.method) << std::endl;
		print_http_uri(request.uri);
		std::cout << "http_version: " << request.http_version << std::endl;
		
		std::cout << "-- headers: -- " << std::endl;
		print_headers(request.headers);

		std::cout << "body: --" << std::endl;
		std::cout << "|" << request.body << "|" << std::endl;
		std::cout << "-- --" << std::endl;
	}

	bool compare_requests(ws::HttpRequest r1, ws::HttpRequest r2)
	{
		if (r1.method == r2.method && r1.uri == r2.uri && r1.headers == r2.headers
			&& r1.body == r2.body)
		{
			return true;
		}

		return false;
	}

} // namespace ws_tester
