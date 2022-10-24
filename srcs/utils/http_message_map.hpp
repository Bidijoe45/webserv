#pragma once

#include <string>
#include <map>

namespace ws
{
    class HttpMessageMap
    {
        public:
            HttpMessageMap();
            std::string get_msg(size_t code) const;
        private:
            std::map<size_t, std::string> messages;
    };
}
