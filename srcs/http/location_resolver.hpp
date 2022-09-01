#include <vector>
#include <string>

#include "../http/http_uri.hpp"
#include "../settings/location.hpp"

namespace ws
{
    class LocationResolver {
        public:
            LocationResolver(const std::vector<Location> &locations);
            Location resolve(const HttpUri &uri);
        private:
            size_t compare_locations(const std::vector<std::string> &vec1, const std::vector<std::string> &vec2);
            std::vector<Location> locations_;
            std::vector<std::vector<std::string> > splitted_locations_;
    };
}
