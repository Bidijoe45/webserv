#include <string>
#include <iostream>
#include "../../../srcs/http/http_request_line_parser.hpp"
#include "../../../srcs/http/http_request_line.hpp"
#include "../../../srcs/http/http_uri.hpp"
#include "../../../srcs/http/http_uri_parser.hpp"

ws::HttpRequestLine generate_model_request_line(ws::HTTP_METHOD method, ws::HttpUri uri, std::string version)
{
	ws::HttpRequestLine req_line;
	req_line.method = method;
	req_line.uri = uri;
	req_line.http_version = version;
	return req_line;
}

int main()
{
	// TEST 0
	ws::HttpRequestLineParser parser0("GET / HTTP/1.1");
	ws::HttpRequestLine req_line0 = parser0.parse();
	if (!parser0.is_valid() || req_line0 != generate_model_request_line(ws::HTTP_METHOD_GET, ws::HttpUriParser("/").parse(), "HTTP/1.1"))
	{
		std::cout << "Failed test: 0"  << std::endl;
		return 1;
	}

	// TEST 1
	ws::HttpRequestLineParser parser1("POST / HTTP/1.1");
	ws::HttpRequestLine req_line1 = parser1.parse();
	if (!parser1.is_valid() || req_line1 != generate_model_request_line(ws::HTTP_METHOD_POST, ws::HttpUriParser("/").parse(), "HTTP/1.1"))
	{
		std::cout << "Failed test: 1"  << std::endl;
		return 1;
	}

	// TEST 2
	ws::HttpRequestLineParser parser2("DELETE / hola1");
	ws::HttpRequestLine req_line2 = parser2.parse();
	if (!parser2.is_valid() || req_line2 != generate_model_request_line(ws::HTTP_METHOD_DELETE, ws::HttpUriParser("/").parse(), "hola1"))
	{
		std::cout << "Failed test: 2"  << std::endl;
		return 1;
	}

	// TEST 3
	ws::HttpRequestLineParser parser3("CACA / HTTP/1.1");
	ws::HttpRequestLine req_line3 = parser3.parse();
	if (!parser3.is_valid() || req_line3 != generate_model_request_line(ws::HTTP_METHOD_INVALID, ws::HttpUriParser("/").parse(), "HTTP/1.1"))
	{
		std::cout << "Failed test: 3"  << std::endl;
		return 1;
	}

	// TEST 4
	ws::HttpRequestLineParser parser4("GET daf/e%C3%B1e HTTP/1.1");
	ws::HttpRequestLine req_line4 = parser4.parse();
	if (!parser4.is_valid() || req_line4 != generate_model_request_line(ws::HTTP_METHOD_GET, ws::HttpUriParser("daf/e%C3%B1e").parse(), "HTTP/1.1"))
	{
		std::cout << "Failed test: 4"  << std::endl;
		return 1;
	}

	// TEST 5
	ws::HttpRequestLineParser parser5("GET /hola.com:3000?query=hola&despedida=adios HTTP/1.0");
	ws::HttpRequestLine req_line5 = parser5.parse();
	if (!parser5.is_valid()
		|| req_line5 != generate_model_request_line(ws::HTTP_METHOD_GET, ws::HttpUriParser("/hola.com:3000?query=hola&despedida=adios").parse(), "HTTP/1.0"))
	{
		std::cout << "Failed test: 5"  << std::endl;
		return 1;
	}

    return 0;
}
