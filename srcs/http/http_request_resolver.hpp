#pragma once

#include "http_request.hpp"
#include "http_response.hpp"
#include "../settings/settings.hpp"

namespace ws
{

    class HttpRequestResolver
    {
        public: 
            HttpRequestResolver(const HttpRequest &request, const ServerSettings &settings);
            HttpResponse resolve();

        private:
            Location resolve_location();
            HttpRequest request_;
            HttpResponse response_;
            ServerSettings settings_;
    };
    
}
