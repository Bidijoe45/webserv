#include <vector>

#include "settings_tests_utils.hpp"

#include "../../../srcs/settings/server_settings.hpp"
#include "../../../srcs/settings/settings.hpp"
#include "../../../srcs/settings/error_page.hpp"
#include "../../../srcs/settings/location.hpp"
#include "../../../srcs/settings/http_methods.hpp"
#include "../../../srcs/settings/rewrite.hpp"
#include "../../../srcs/settings/parser/settings_parser.hpp"

namespace ws_tester
{

ws::Settings prepare_settings() {
	
	//Settings
	ws::Settings settings;

	//Servers
	ws::ServerSettings server_settings1;

	server_settings1.port = 80;
	server_settings1.server_name = "test_server_1";

	//Error pages
	ws::ErrorPage err_page1;
	
	err_page1.code = 404;
	err_page1.path = "/404.html";

	server_settings1.error_pages.push_back(err_page1);

	//Client max body size
	server_settings1.client_max_body_size = 1024;

	settings.servers.push_back(server_settings1);

	return settings;
}

bool settings_test_1(size_t test_n, bool print) {

	ws::Settings prepared_settings = prepare_settings();
	
	if (print) {
		std::cout << "-- Prepared settings -- "<< std::endl;
		print_settings(prepared_settings);
	}

	ws::SettingsParser settings_parser("./tester/settings/test_files/conf_file1.conf");
	ws::Settings parsed_settings = settings_parser.parse();

	if (print) {
		std::cout << "-- Parsed settings --" << std::endl;
		print_settings(parsed_settings);
	}

	return prepared_settings == parsed_settings;
}

}