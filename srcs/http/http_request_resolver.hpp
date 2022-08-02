#pragma once

#include "http_request.hpp"
#include "http_response.hpp"

namespace ws
{

    class HttpRequestResolver
    {
        public: 
            HttpRequestResolver(const HttpRequest &request);
            std::string to_string();

        private:
            void resolve();
            HttpRequest request_;
            HttpResponse response_;

    };
    
}
