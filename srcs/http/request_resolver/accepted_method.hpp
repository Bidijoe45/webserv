#pragma once

#include "request_handler.hpp"
#include "../../settings/server_settings.hpp"

namespace ws
{
    class AcceptedMethod : public RequestHandler
    {
        public:
            AcceptedMethod(HttpRequest *request);
            RequestHandlerPayload *handle(RequestHandlerPayload *payload);

        private:
            HttpRequest *request_;
    };
}
