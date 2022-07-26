#include "http_uri_parser.hpp"

#include <string>

#include "http_uri.hpp"

namespace ws
{
    HttpUriParser::HttpUriParser(const std::string &uri) : line_(uri) {}

    HttpUri HttpUriParser::parse()
    {
        HttpUri uri;
        
        uri.host = this->line_;

        return uri;
    }

    HTTP_URI_ERROR HttpUriParser::get_error()
    {
        return this->error_;
    }

    bool HttpUriParser::uri_is_valid()
    {
        return (this->error_ == HTTP_URI_VALID);
    }


}
