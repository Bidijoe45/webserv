#pragma once

#include "request_handler.hpp"
#include "../../utils/content_type_map.hpp"

namespace ws
{
    class MethodCaller : public RequestHandler
    {
        public:
            MethodCaller(HttpRequest *request, const ContentTypeMap &content_type_map);
            RequestHandlerPayload *handle(RequestHandlerPayload *payload);

        private:
            HttpRequest *request_;
            ContentTypeMap content_type_map_;

    };
}
