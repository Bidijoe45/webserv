#pragma once

#include "../../../srcs/http/http_request.hpp"

namespace ws_tester
{
	void print_http_request(const ws::HttpRequest &request);
	bool compare_requests(ws::HttpRequest r1, ws::HttpRequest r2);
} // namespace ws_tester
