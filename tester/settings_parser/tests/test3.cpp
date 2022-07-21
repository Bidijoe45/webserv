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

/**

Al archivo conf_file4.conf le falta un punto y coma

*/
bool settings_test_3(size_t test_n, bool print) {
	
	ws::SettingsParser settings_parser("./tester/settings_parser/test_files/conf_file4.conf");
	ws::Settings parsed_settings = settings_parser.parse();

	if (print) {
		std::cout << "Error msg should be: Missing semicolon" << std::endl;
		std::cout << "Error msg: " << settings_parser.get_error_msg() << std::endl;
	}
	
	return !settings_parser.is_valid();
}

}