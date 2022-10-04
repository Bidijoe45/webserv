#pragma once

#include "../http_response.hpp"
#include "../http_request.hpp"
#include "../../settings/server_settings.hpp"
#include "request_handler.hpp"

namespace ws
{
    class Resolver
    {
        public:
            Resolver(const ServerSettings &server_settings, const HttpRequest &request);
            HttpResponse *get_response();
            void resolve();

        private:
            RequestHandlerPayload *payload_;
            ServerSettings settings_;
            HttpRequest request_;
    };
}


