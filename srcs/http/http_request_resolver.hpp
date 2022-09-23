#pragma once

#include <sys/socket.h>

#include "http_request.hpp"
#include "http_response.hpp"
#include "../settings/settings.hpp"
#include "../settings/location.hpp"
#include "../server/file_system.hpp"
#include "../utils/env_map.hpp"
#include "connection.hpp"
#include "content_type_map.hpp"

namespace ws
{

    class HttpRequestResolver
    {
        public: 
            HttpRequestResolver(const HttpRequest &request, const ServerSettings &settings, const EnvMap &env, const Connection &connection, const ContentTypeMap &content_types);
            HttpResponse resolve();

        private:
			std::string resolve_status_code();
			void apply_method();
			void apply_get_method();
			void apply_post_method();
			void apply_delete_method();
            std::string generate_autoindex(const FileSystem &file);
            void set_error_body();
            std::string find_error_page();
            std::string create_default_error_page();
            std::string resolve_custom_error_page(const std::string error_page_path);
			std::string resolve_cgi_executable();
			std::string resolve_content_type(std::string extension);

            HttpRequest request_;
            HttpResponse response_;
            ServerSettings settings_;
			Location location_;
			std::string file_path_;
			EnvMap env_;
			Connection connection_;
			ContentTypeMap content_types_;
    };
    
}
