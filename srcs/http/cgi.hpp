#pragma once

#include <vector>
#include <string>

#include "http_request.hpp"
#include "env_map.hpp"

namespace ws {

    class CGI
    {
        public:
            CGI();
            void set_env(const EnvMap &env, const std::string &file_path, const HttpRequest &request);
			void execute(const std::string &file_path);
			void set_executable(const std::string &executable);
			const std::string get_executable() const;

        private:
			EnvMap env_;
			std::string executable_;

			void delete_double_pointer(char **envp);
    };

}
