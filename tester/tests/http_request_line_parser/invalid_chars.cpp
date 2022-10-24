#include <string>
#include "../../../srcs/http/http_request_line_parser.hpp"
#include "../../../srcs/http/http_request_line.hpp"

int main()
{
	ws::HttpRequestLineParser parser("GET \r/ HTTP/1.1");
	ws::HttpRequestLine req_line = parser.parse();

	if (parser.is_valid())
		return 1;

    return 0;
}
