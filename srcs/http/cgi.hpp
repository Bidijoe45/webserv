#pragma once

#include <vector>
#include <string>

#include "http_request.hpp"
#include "../utils/env_map.hpp"
#include "http_header_map.hpp"
#include "data_buffer.hpp"

namespace ws {

    class CGI
    {
        public:
            CGI(const std::string &executable, const EnvMap &env, const std::string &file_path, const HttpRequest &request);
			unsigned int execute();
			HttpHeaderMap get_header_map();
			std::string get_body();

		private:
            void set_env();
			//unsigned int exit_with_error(unsigned int code, char **envp);
			void parse_execution_output();
			HttpHeaderMap parse_headers();
			std::string parse_body();

			EnvMap env_;
			std::string executable_;
			std::string file_path_;
			HttpRequest request_;

			std::string execution_output_;
			DataBuffer output_buff_;
			HttpHeaderMap response_headers_;
			std::string response_body_;
			unsigned int status_code_;
    };

}
