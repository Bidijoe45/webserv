#include "../../utils/settings_tests_utils.hpp"
#include "../../../srcs/settings/parser/settings_parser.hpp"
#include "../../../srcs/settings/settings.hpp"
#include "../../../srcs/http/http_uri_parser.hpp"

ws::Settings prepare_settings()
{
    ws::Settings settings;


    //Server settings 1
    ws::ServerSettings ss;
    
    //Port
    ss.port = 3000;

    //Server name
    ss.server_name = "webserv";

    //Error pages
    ws::ErrorPage error_page1;

    error_page1.code = 404;
    error_page1.path = "path/to/file";

    ss.error_pages.push_back(error_page1);

    //Client max body size
    ss.client_max_body_size = 1024;

    //Locations
    
    //Location 1
    ws::Location location1;

    location1.path = "/location/path";
    location1.root = "root/directive/path";
    location1.index = "index_file_name";
    location1.redirect.code = 301;
    location1.redirect.to = ws::HttpUriParser("redirect/directive/path").parse();
    location1.autoindex = true;
    location1.methods.push_back(ws::HTTP_METHOD_GET);
    location1.methods.push_back(ws::HTTP_METHOD_POST);

    ss.locations.push_back(location1);

    //Location 2
    ws::Location location2;

    location2.path = "/location2/path";
    location2.root = "/path/of/root";
    location2.methods.push_back(ws::HTTP_METHOD_DELETE);
    location2.methods.push_back(ws::HTTP_METHOD_GET);
    location2.redirect.code = 302;
    location2.redirect.to = ws::HttpUriParser("redirect2/directive/path").parse();
    location2.autoindex = false;

    ws::CGISettings cgi1;

    cgi1.extension = ".extension";
    cgi1.executable = "/path/of/cgi/executable";

    location2.cgis.push_back(cgi1);

    ws::CGISettings cgi2;

    cgi2.extension = ".extension2";
    cgi2.executable = "/path/of/cgi2/executable2";

    location2.cgis.push_back(cgi2);

    ss.locations.push_back(location2);

    settings.servers.push_back(ss);

    //Server settings 2
    ws::ServerSettings ss2;

    //Port
    ss2.port = 3001;

    //Server name
    ss2.server_name = "webserv2";

    //Locations
    
    //Location 1
    ws::Location location3;

    location3.path = "/location3/path";
    location3.root = "root/directive/path";
    location3.autoindex = true;
    location3.methods.push_back(ws::HTTP_METHOD_GET);

    ss2.locations.push_back(location3);
    
    settings.servers.push_back(ss2);

    return settings;
}

int main(int argc, char **argv, char **envp) {
    ws::SettingsParser settings_parser("tests/settings_parser/test_files/all_directives.conf");
    ws::Settings parsed_settings = settings_parser.parse();
    ws::Settings expected_settings = prepare_settings();

    if (!settings_parser.is_valid())
    {
        std::cout << settings_parser.get_error_msg() << std::endl;
        return 1;
    }
    
    std::cout << "-- Expected settings --" << std::endl;
    print_settings(expected_settings);
    std::cout << "-- Parsed settings --" << std::endl;
    print_settings(parsed_settings);

    if (expected_settings == parsed_settings)
        return 0;

    return 1;
}
