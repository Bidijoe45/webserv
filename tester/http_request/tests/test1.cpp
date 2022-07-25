#include <typeinfo>
#include <iostream>

#include "../../../srcs/http/http_request.hpp"

namespace ws_tester
{

ws::HttpRequest preaprae_request()
{
	ws::HttpRequest request;

	request.method = ws::HTTP_METHOD_GET;
	request.http_version = "HTTP/1.1";

	return request;
}

bool http_request_test1(size_t test_n, bool print)
{
	ws::HttpRequest request = preaprae_request();

	request.validate();

	return request.is_valid();
}

}
