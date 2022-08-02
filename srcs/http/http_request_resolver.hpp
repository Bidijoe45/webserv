#pragma once

#include "http_request.hpp"
#include "http_response.hpp"

namespace ws
{

    class HttpRequestResolver
    {
        public: 
            HttpRequestResolver(const HttpRequest &request);
            void resolve();

        private:
            HttpRequest request_;
            HttpResponse response_;

    };
    
}
