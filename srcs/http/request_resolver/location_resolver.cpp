#include "location_resolver.hpp"
#include "../../utils/string_utils.hpp"

namespace ws
{

    LocationResolver::LocationResolver(HttpRequest *request, const std::vector<Location> &locations)
        : RequestHandler(), request_(request)
    {
        std::vector<Location>::const_iterator it = locations.begin();
        std::vector<Location>::const_iterator ite = locations.end();

        for (; it != ite; it++)
        {
            std::vector<std::string> split = string_split((*it).path, "/");
            this->splitted_locations_.push_back(split);
        }
        this->locations_ = locations;
    }

    RequestHandlerPayload *LocationResolver::handle(RequestHandlerPayload *payload)
    {
        payload->location = resolve(this->request_->request_line.uri);
        
        if (payload->location.path.size() == 0)
            throw RequestHandler::Exception(404);

        return RequestHandler::handle(payload);
    }

    Location LocationResolver::resolve(const HttpUri &uri)
    {
        std::vector<std::string> split_uri_path = string_split(uri.path, "/");
        Location location;
        size_t location_i = 0;
        size_t highest_score = 0;

        if (this->locations_.size() == 0)
            throw RequestHandler::Exception(403);

        location = *this->locations_.begin();

        for (; location_i < this->locations_.size(); location_i++)
        {
            size_t score = compare_split_paths(split_uri_path, this->splitted_locations_[location_i]);

            if (this->splitted_locations_[location_i].size() == 0 && highest_score == 0)
                location = Location(this->locations_[location_i]);

            if (score > highest_score)
            {
                location = Location(this->locations_[location_i]);
                highest_score = score;
            }
        }

        return location;
    }
}
