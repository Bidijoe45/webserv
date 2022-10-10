#pragma once

#include "../http_response.hpp"
#include "../http_request.hpp"
#include "../../settings/location.hpp"

namespace ws
{
    struct RequestHandlerPayload
    {
        RequestHandlerPayload() {}

        HttpResponse response;
        Location location;
        std::string file_path;
    };
}
