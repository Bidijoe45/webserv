#include <vector>

#include <iostream>

#include "location_resolver.hpp"
#include "../utils/string_utils.hpp"
#include "../http/http_uri.hpp"
#include "../settings/location.hpp"

namespace ws
{

    LocationResolver::LocationResolver(const std::vector<Location> &locations)
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

    Location LocationResolver::resolve(const HttpUri &uri)
    {
        std::vector<std::string> split_uri_path = string_split(uri.path, "/");
        Location location;
        size_t location_i = 0;
        size_t splitted_location_i = 0;
        size_t hightes_score = 0;

        for (; location_i < this->locations_.size(); location_i++)
        {
            size_t score = this->compare_locations(split_uri_path, this->splitted_locations_[location_i]);
            if (score > hightes_score)
                location = this->locations_[location_i];
        }
        
        return location;
    }
    
    size_t LocationResolver::compare_locations(const std::vector<std::string> &vec1, const std::vector<std::string> &vec2)
    {
        size_t size = (vec1.size() < vec2.size()) ? vec1.size() : vec2.size();
        size_t equal_segments = 0;

         if (vec2.size() == 0)
             equal_segments += 1;
        
         for (size_t i=0; i < size; i++)
         {
             if (vec1[i] == vec2[i])
                 equal_segments += 2;
             else
                 return equal_segments;
         }

        return equal_segments;
    }
}
