#pragma once

#include <vector>
#include <string>

#include "http_request.hpp"
#include "env_map.hpp"
#include "http_header_map.hpp"

namespace ws {

    class CGI
    {
        public:
            CGI(const std::string &executable, const EnvMap &env, const std::string &file_path, const HttpRequest &request);
			unsigned int execute(const std::string &file_path);
			//HttpHeaderMap get_header_map();
			//std::string get_body();

		private:
			unsigned int exit_with_error(unsigned int code, char **envp);
            void set_env();
			std::string get_cgi_header_name(std::string line);
			std::string get_cgi_header_value(std::string line);
			void parse_execution_output();
			HttpHeaderMap parse_cgi_headers();
			std::string parse_cgi_body();


			EnvMap env_;
			std::string executable_;
			std:::string file_path_;
			HttpRequest request_;

			std::string execution_output_;
			HttpHeaderMap response_headers_;
			std::string response_body_;
			unsigned int status_code_;
    };

}
