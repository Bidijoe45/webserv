#include "http_message_map.hpp"

#include <string>
#include <map>

namespace ws
{
    HttpMessageMap::HttpMessageMap()
    {
        this->messages.insert(std::make_pair(200, "OK"));
        this->messages.insert(std::make_pair(201, "Created"));
        this->messages.insert(std::make_pair(301, "Moved Permanently"));
        this->messages.insert(std::make_pair(302, "Found"));
        this->messages.insert(std::make_pair(400, "Bad Request"));
        this->messages.insert(std::make_pair(403, "Forbidden"));
        this->messages.insert(std::make_pair(404, "Not Found"));
        this->messages.insert(std::make_pair(405, "Method Not Allowed"));
        this->messages.insert(std::make_pair(408, "Request Timeout"));
        this->messages.insert(std::make_pair(411, "Length Required"));
        this->messages.insert(std::make_pair(414, "URI Too Long"));
        this->messages.insert(std::make_pair(500, "Internal Server Error"));
        this->messages.insert(std::make_pair(501, "Not Implemented"));
        this->messages.insert(std::make_pair(501, "Not Implemented"));
        this->messages.insert(std::make_pair(505, "Http Version Not Supported"));
    }

    std::string HttpMessageMap::get_msg(size_t code) const
    {
        std::map<size_t, std::string>::const_iterator it = this->messages.find(code);
        if (it == this->messages.end())
            return "";
        return it->second;
    }
}
