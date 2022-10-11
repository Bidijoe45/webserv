#include "../../utils/tester_utils.hpp"
#include "../../../srcs/http/http_parser.hpp"
#include "../../../srcs/http/http_request.hpp"
#include "../../../srcs/server/data_buffer.hpp"

ws::HttpRequest generate_model_request()
{
	ws::HttpRequest http_request;
	return http_request;
}

int main()
{
	ws::HttpParser http_parser;
	ws::DataBuffer buff("");
	http_parser.parse(buff);
	ws::HttpRequest model_request = generate_model_request();
	ws::HttpRequest http_request = http_parser.get_request();

	if (http_request.error != ws::HttpRequest::NO_ERROR)
        return 1;
	if (http_parser.get_stage() != ws::HttpParser::REQUEST_LINE)
		return 1;
	if (!(http_request == model_request))
		return 1;

    return 0;
}
