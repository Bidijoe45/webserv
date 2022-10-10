#pragma once

#include "http_method.hpp"

namespace ws
{
    class HttpDeleteMethod : public HttpMethod
    {
        public:
            HttpDeleteMethod(HttpRequest *request, const Location &location, const std::string &file_path, const ContentTypeMap &content_type_map);
            HttpResponse execute();
    };
}
