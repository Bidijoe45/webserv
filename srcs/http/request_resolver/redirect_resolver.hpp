#pragma once

#include "request_handler.hpp"
#include "../headers/http_headers.hpp"

namespace ws
{
    class RedirectResolver : public RequestHandler
    {
        public:
            RedirectResolver();
            RequestHandlerPayload *handle(RequestHandlerPayload *payload);
    };
}
