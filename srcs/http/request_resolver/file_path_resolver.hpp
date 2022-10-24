#pragma once

#include "request_handler.hpp"
#include "../../server/file_system.hpp"

namespace ws
{
    class FilePathResolver : public RequestHandler
    {
        public:
            FilePathResolver(HttpRequest *request);
            RequestHandlerPayload *handle(RequestHandlerPayload *payload);
        
        private: 
            std::string get_file_path(RequestHandlerPayload *payload);

            HttpRequest *request_;

    };
}
