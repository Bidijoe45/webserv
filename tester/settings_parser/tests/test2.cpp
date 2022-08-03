#include <vector>

#include "settings_tests_utils.hpp"

#include "../../../srcs/http/http_request.hpp"
#include "../../../srcs/settings/server_settings.hpp"
#include "../../../srcs/settings/settings.hpp"
#include "../../../srcs/settings/error_page.hpp"
#include "../../../srcs/settings/location.hpp"
#include "../../../srcs/settings/rewrite.hpp"
#include "../../../srcs/settings/parser/settings_parser.hpp"

namespace ws_tester
{

static ws::Settings prepare_settings() {
	
	//Settings
	ws::Settings settings;

	//Servers
	ws::ServerSettings server_settings1;

	server_settings1.port = 80;
	server_settings1.server_name = "test_server_1";

	//Locations
	ws::Location location1;

	location1.path = "/";
	location1.root = "/www/html/mi_web";
	location1.index = "index.html";
	location1.methods.push_back(ws::HTTP_METHOD_POST);
	location1.methods.push_back(ws::HTTP_METHOD_DELETE);

	//Rewrite
	ws::Rewrite rw1;

	rw1.from = "/find";
	rw1.to = "/search";
	rw1.permanent = true;

	location1.rewrites.push_back(rw1);

	location1.autoindex = true;

	server_settings1.locations.push_back(location1);
	server_settings1.client_max_body_size = 0;

	settings.servers.push_back(server_settings1);

	return settings;
}

bool settings_test_2(size_t test_n, bool print) {

	ws::Settings prepared_settings = prepare_settings();
	
	if (print) {
		std::cout << "-- Prepared settings -- "<< std::endl;
		print_settings(prepared_settings);
	}

	ws::SettingsParser settings_parser("./tester/settings_parser/test_files/conf_file2.conf");
	ws::Settings parsed_settings = settings_parser.parse();

	bool file_is_valid = settings_parser.is_valid();

	if (print) {
		std::cout << "-- Parsed settings --" << std::endl;
		std::cout << "File is valid: " << ( (file_is_valid) ? "true" : "false" ) << std::endl;
		print_settings(parsed_settings);
	}

	return (prepared_settings == parsed_settings && file_is_valid == true);
}

}
