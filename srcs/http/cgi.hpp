#pragma once

#include "http_request.hpp"

namespace ws {

    class CGI
    {
        public:
            CGI(const HttpRequest &request);
            void execute();
        private:
    };

}
