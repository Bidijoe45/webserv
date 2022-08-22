#include "settings.hpp"
#include "http_header.hpp"
#include "http_headers.hpp"
#include "http_header_map.hpp"

namespace ws
{
    ServerSettings Settings::resolve_settings_hostname(const HttpRequest &request, int port)
    {
        ServerSettings settings;
        bool settings_set = false;
        std::vector<ServerSettings>::const_iterator servers_it = this->servers.begin();
        std::vector<ServerSettings>::const_iterator servers_ite = this->servers.end();
        HttpHeaderHost *host_header;

        HttpHeaderMap::const_iterator headers_iterator = request.headers.find("host");
        if (headers_iterator == request.headers.end())
        {
            settings = *servers_it;
            return settings;
        }

        host_header = dynamic_cast<HttpHeaderHost *>((*headers_iterator).second);
        
        for (; servers_it != servers_ite; servers_it++)
        {
            if (host_header->value == (*servers_it).server_name
                && (*servers_it).port == port)
                return *servers_it;

            if ((*servers_it).port == port && settings_set == false)
            {
                settings = *servers_it;
                settings_set = true;
            }
        }

		return settings;
    }

	bool operator==(const Settings &lhs, const Settings &rhs) {
		return lhs.servers == rhs.servers;
	}
} // namespace ws
