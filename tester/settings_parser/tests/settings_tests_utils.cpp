#include <iostream>

#include "settings_tests_utils.hpp"
#include "../../../srcs/http/http_request.hpp"
#include "../../../srcs/settings/server_settings.hpp"
#include "../../../srcs/settings/settings.hpp"
#include "../../../srcs/settings/error_page.hpp"
#include "../../../srcs/settings/location.hpp"
#include "../../../srcs/settings/rewrite.hpp"


void print_methods(std::vector<ws::HTTP_METHOD> &methods) {
	for (int i=0; i < methods.size(); i++) {

		switch (methods[i])
		{
		case ws::HTTP_METHOD_GET:
			std::cout << "GET";
			break;

		case ws::HTTP_METHOD_POST:
			std::cout << "POST";
			break;

		case ws::HTTP_METHOD_DELETE:
			std::cout << "DELETE";
			break;
		default:
			std::cout << "INVALID";
			break;
		}

		std::cout << " ";
	}
}

void print_rewrite(std::vector<ws::Rewrite> &rewrites) {
	for (int i=0; i < rewrites.size(); i++) {
		std::cout << " -- Rewrite --" << std::endl;
		std::cout << " from " << rewrites[i].from << std::endl;
		std::cout << " to: " << rewrites[i].to << std::endl;
		std::cout << " permanent: " << rewrites[i].permanent << std::endl;
	}
}

void print_error_pages(std::vector<ws::ErrorPage> &error_pages) {
	for (int i=0; i < error_pages.size(); i++) {
		std::cout << " -- Page --" << std::endl;
		std::cout << " code: " << error_pages[i].code << std::endl;
		std::cout << " path: " << error_pages[i].path << std::endl;
	}
}

void print_locations(std::vector<ws::Location> &locations) {
	for (int i=0; i < locations.size(); i++) {
		std::cout << " -- Location --" << std::endl;
		std::cout << " path: " << locations[i].path << std::endl;
		std::cout << " root: " << locations[i].root << std::endl;
		std::cout << " index: " << locations[i].index << std::endl;
		std::cout << " autoindex: " << (locations[i].autoindex ? "on" : "off") << std::endl;
		std::cout << " Accepted methods: ";
		print_methods(locations[i].methods);
		std::cout << std::endl;
		std::cout << " -- -- -- --" << std::endl;
	}
}

void print_server_settings(ws::ServerSettings &server_settings) {
	std::cout << "port: " << server_settings.port << std::endl;
	std::cout << "server_name: " << server_settings.server_name << std::endl;
	std::cout << "client_max_body_size: " << server_settings.client_max_body_size << std::endl;
	std::cout << "-- Error pages --" << std::endl;
	print_error_pages(server_settings.error_pages);
	std::cout << "-- Locations --" << std::endl;
	print_locations(server_settings.locations);
}

void print_settings(ws::Settings &settings) {

	std::cout << "----- SETTINGS FILE -----" << std::endl;

	for (size_t i=0; i < settings.servers.size(); i++) {
		std::cout << "-- SERVER --" << std::endl;
		print_server_settings(settings.servers[i]);
		std::cout << "-- -- -- --" << std::endl;
	}

	std::cout << "------------------------" << std::endl;

}
