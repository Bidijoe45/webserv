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
            CGI();
            void set_env(const EnvMap &env, const std::string &file_path, const HttpRequest &request);
			unsigned int execute(const std::string &file_path);
			void set_executable(const std::string &executable);
			const std::string get_executable() const;
			HttpHeaderMap parse_cgi_headers();
			std::string parse_cgi_body();

        private:
			EnvMap env_;
			std::string executable_;
			std::string execution_output_;

			unsigned int exit_with_error(unsigned int code, char **envp);
			std::string get_cgi_header_name(std::string line);
			std::string get_cgi_header_value(std::string line);
    };

}
