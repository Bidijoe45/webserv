#pragma once

#include "http_request.hpp"
#include "http_response.hpp"
#include "../settings/settings.hpp"
#include "../settings/location.hpp"
#include "../server/file_system.hpp"

namespace ws
{

    class HttpRequestResolver
    {
        public: 
            HttpRequestResolver(const HttpRequest &request, const ServerSettings &settings);
            HttpResponse resolve();

        private:
            Location resolve_location();
			std::string resolve_status_code();
			void apply_method();
			void apply_get_method();
			void apply_post_method();
			void apply_delete_method();
            std::string generate_autoindex(const FileSystem &file);
            HttpRequest request_;
            HttpResponse response_;
            ServerSettings settings_;
			Location location_;
			std::string file_path_;
    };
    
}
