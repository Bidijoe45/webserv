#pragma once

#include "request_handler.hpp"

namespace ws
{
    class SupportedMethods: public RequestHandler
    {
        public:
            SupportedMethods(HttpRequest *request);
            RequestHandlerPayload *handle(RequestHandlerPayload *payload);

        private:
            HttpRequest *request_;
            std::vector<HTTP_METHOD> supported_methods_;
    };
}
