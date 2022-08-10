#include "../http/http_header.hpp"
#include "settings.hpp"

namespace ws
{
    ServerSettings Settings::resolve_settings_hostname(const HttpRequest &request, int port)
    {
        ServerSettings settings;
        bool settings_set = false;
        HttpHeaderMap::const_iterator headers_iterator = request.headers.find("host");
        HttpHeaderHost *host_header = dynamic_cast<HttpHeaderHost *>((*headers_iterator).second);
        std::vector<ServerSettings>::const_iterator servers_it = this->servers.begin();
        std::vector<ServerSettings>::const_iterator servers_ite = this->servers.end();
        
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
