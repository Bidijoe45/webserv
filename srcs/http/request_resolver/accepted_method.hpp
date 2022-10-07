#pragma once

#include "request_handler.hpp"
#include "../../settings/server_settings.hpp"

namespace ws
{
    class AcceptedMethod : public RequestHandler
    {
        public:
            AcceptedMethod(HttpRequest *request, ServerSettings *settings);
            RequestHandlerPayload *handle(RequestHandlerPayload *payload);

        private:
            HttpRequest *request_;
            ServerSettings *settings_;

    };
}
