#pragma once

#include "http_request.hpp"
#include "http_response.hpp"
#include "../settings/server_settings.hpp"

namespace ws
{

    class HttpRequestResolver
    {
        public: 
            HttpRequestResolver(const HttpRequest &request, const ServerSettings &settings);
            HttpResponse resolve();

        private:
            HttpRequest request_;
            HttpResponse response_;
            ServerSettings settings_;

    };
    
}
