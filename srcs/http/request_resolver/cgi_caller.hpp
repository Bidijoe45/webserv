#pragma once

#include "request_handler.hpp"
#include "../../utils/env_map.hpp"
#include "../../server/connection.hpp"
#include "../cgi.hpp"

namespace ws
{
    class CGICaller : public RequestHandler
    {
        public:
            CGICaller(HttpRequest *request, EnvMap *env_map, Connection *connection);
            RequestHandlerPayload *handle(RequestHandlerPayload *payload);

        private:
            std::string get_uri_file_extension(const std::string &file_path);
            std::string get_cgi_executable(const std::string &file_path, const Location &location);

            HttpRequest *request_;
            EnvMap *env_map_;
            Connection *connection_;
    };
}
