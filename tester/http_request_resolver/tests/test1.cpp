#include <typeinfo>
#include <iostream>

#include "../../../srcs/http/http_request_resolver.hpp"
#include "../../../srcs/http/http_request.hpp"
#include "../../../srcs/settings/server_settings.hpp"

namespace ws_tester
{

	ws::ServerSettings prepare_settings()
	{
		ws::ServerSettings settings;

		settings.server_name = "localhost";

		return settings;
	}

	ws::HttpRequest prepare_request()
	{
		ws::HttpRequest request;

		request.method = ws::HTTP_METHOD_GET;
		request.uri.path = "/index.html";

		return request;
	}

	bool http_request_resolver_test1(size_t test_n, bool print)
	{
		ws::ServerSettings settings = prepare_settings();
		ws::HttpRequest request = prepare_request();
		ws::HttpRequestResolver request_resolve(request, settings);

		return true;
	}

}
