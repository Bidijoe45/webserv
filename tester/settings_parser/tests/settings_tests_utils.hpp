#pragma once

#include <iostream>

#include "../../../srcs/http/http_request.hpp"
#include "../../../srcs/settings/server_settings.hpp"
#include "../../../srcs/settings/settings.hpp"
#include "../../../srcs/settings/error_page.hpp"
#include "../../../srcs/settings/location.hpp"
#include "../../../srcs/settings/rewrite.hpp"


void print_methods(std::vector<ws::HTTP_METHOD> &methods);
void print_rewrite(std::vector<ws::Rewrite> &rewrites);
void print_error_pages(std::vector<ws::ErrorPage> &error_pages);
void print_locations(std::vector<ws::Location> &locations);
void print_server_settings(ws::ServerSettings &server_settings);
void print_settings(ws::Settings &settings);
