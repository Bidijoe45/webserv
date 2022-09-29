#include "../../utils/tester_utils.hpp"
#include "../../../srcs/settings/parser/settings_parser.hpp"
#include "../../../srcs/settings/settings.hpp"

#include <iostream>

int main() {

    ws::SettingsParser settings_parser("tests/settings_parser/test_files/listen_no_numeric.conf");
    ws::Settings parsed_settings = settings_parser.parse();

    std::cout << settings_parser.get_error_msg() << std::endl;

    if (settings_parser.is_valid())
        return KO;

    return OK;
}
