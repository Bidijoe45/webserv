#pragma once

#include "../http_response.hpp"
#include "../http_request.hpp"
#include "../../settings/server_settings.hpp"
#include "../../utils/env_map.hpp"
#include "../../server/connection.hpp"
#include "../../utils/content_type_map.hpp"
#include "request_handler.hpp"

namespace ws
{
    class RequestResolver
    {
        public:
            RequestResolver(const ServerSettings &server_settings,
                        const HttpRequest &request,
                        const EnvMap &env_map,
                        const Connection &connection,
                        const ContentTypeMap &content_type_map);
            ~RequestResolver();
            HttpResponse get_response();

        private:
            void resolve();
            RequestHandlerPayload *payload_;
            ServerSettings settings_;
            HttpRequest request_;
            Connection connection_;
            EnvMap env_map_;
            ContentTypeMap content_type_map_;
    };
}


