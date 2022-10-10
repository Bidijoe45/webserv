#pragma once

#include "request_handler.hpp"

namespace ws
{
    class RequestErrorChecker : public RequestHandler
    {
        public:
            RequestErrorChecker(HttpRequest *request);
            RequestHandlerPayload *handle(RequestHandlerPayload *payload);

        private:
            HttpRequest *request_;
    };
}
