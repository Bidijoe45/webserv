#pragma once

#include "../http_response.hpp"
#include "../http_request.hpp"
#include "../../settings/location.hpp"

namespace ws
{
    struct RequestHandlerPayload
    {
        RequestHandlerPayload() {
            this->response = new HttpResponse();
            this->location = NULL;
        }

        HttpResponse *response;
        Location *location;
    };
}
