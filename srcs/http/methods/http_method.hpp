#pragma once

#include "../http_request.hpp"
#include "../http_response.hpp"
#include "../headers/http_headers.hpp"
#include "../../utils/content_type_map.hpp"
#include "../../settings/location.hpp"
#include "../../server/file_system.hpp"
#include "../http_multipart_body.hpp"
#include "../http_multipart_body_parser.hpp"

namespace ws 
{
    class HttpMethod
    {
        public:
            HttpMethod(HttpRequest *request, const Location &location, const std::string &file_path, const ContentTypeMap &content_type_map);
            virtual ~HttpMethod() {}
            virtual HttpResponse execute() = 0;

            class Exception
            {
                public:
                    Exception(size_t error_code);
                    const char * what ();
                    size_t get_error_code();

                private:
                    size_t error_code_;
            };

        protected:
            std::string resolve_file_extension_content_type(std::string file_extension);
            std::string generate_autoindex(const FileSystem &file);
            HttpResponse get_response();
        
            HttpRequest *request_;
            Location location_;
            HttpResponse response_;
            std::string file_path_;
            ContentTypeMap content_type_map_;
    };
}
