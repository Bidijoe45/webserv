#pragma once

#include "request_handler.hpp"
#include "../../utils/string_utils.hpp"

namespace ws
{

    class LocationResolver: public RequestHandler
    {
        public:
            LocationResolver(HttpRequest *request, const std::vector<Location> &locations);
            RequestHandlerPayload *handle(RequestHandlerPayload *payload);

        private:
            Location resolve(const HttpUri &uri);
            size_t compare_locations(const std::vector<std::string> &vec1, const std::vector<std::string> &vec2);

            std::vector<Location> locations_;
            std::vector<std::vector<std::string> > splitted_locations_;
            HttpRequest *request_;
    };
}
