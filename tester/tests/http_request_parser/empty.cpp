#include "../../../srcs/http/http_request.hpp"
#include "../../../srcs/server/connection.hpp"
#include "../../../srcs/server/server.hpp"
#include "../../../srcs/settings/parser/settings_parser.hpp"

ws::HttpRequest generate_model_request()
{
	ws::HttpRequest http_request;
	return http_request;
}

int main()
{
	ws::Connection connection;
	connection.port = 3000;
	connection.buff.append("");

	ws::Server server;

	ws::SettingsParser settings_parser("./tests/http_request_parser/server.conf");
	server.set_settings(settings_parser.parse());
	if (!settings_parser.is_valid())
	{
		std::cout << "server config file invalid: " << settings_parser.get_error_msg() << std::endl;
		return 1;
	}

	server.parse_request(connection);

	ws::HttpRequest model_request = generate_model_request();
	ws::HttpRequest http_request = connection.http_parser.get_request();

	if (http_request.error != ws::HttpRequest::NO_ERROR)
        return 1;
	if (connection.http_parser.get_stage() != ws::HttpParser::REQUEST_LINE)
		return 1;
	if (!(http_request == model_request))
		return 1;

    return 0;
}
