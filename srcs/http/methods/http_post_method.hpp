#pragma once

#include "http_method.hpp"
#include "../http_request.hpp"
#include "../http_response.hpp"
#include "../headers/http_headers.hpp"
#include "../../utils/content_type_map.hpp"
#include "../../settings/location.hpp"
#include "../../server/file_system.hpp"
#include "../http_multipart_body.hpp"
#include "../http_multipart_body_parser.hpp"

namespace ws
{
    class HttpPostMethod : public HttpMethod
    {
        public:
            HttpPostMethod(HttpRequest *request, const Location &location, const std::string &file_path, const ContentTypeMap &content_type_map);
            HttpResponse execute();
    };

}
