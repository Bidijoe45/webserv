#pragma once

#include "../../settings/server_settings.hpp"
#include "../http_response.hpp"
#include "../../server/file_system.hpp"
#include "../../utils/string_utils.hpp"
#include "../headers/http_headers.hpp"
#include "../../utils/http_message_map.hpp"

namespace ws
{
    
    class ErrorResponseGenerator
    {
        public: 
            ErrorResponseGenerator(const std::vector<ErrorPage> &error_pages, const HttpMessageMap &http_message_map);
            HttpResponse get(size_t error_code);
	        void generate_body();
	        std::string get_custom_error_page_path();
	        std::string create_default_error_page();
	        std::string get_custom_error_page(const std::string error_page_path);

        private:
            HttpResponse response_;
            std::vector<ErrorPage> error_pages_;
            const HttpMessageMap &http_message_map_;
    };

}
