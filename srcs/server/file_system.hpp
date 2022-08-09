#pragma once

#include <string>

namespace ws
{
    class FileSystem
    {
        public:
            FileSystem();
            FileSystem(std::string file_path);
            bool open(std::string file_path);
            bool is_open();
            bool is_dir();
            std::string get_path();
        
        private:
            std::string path_;
            bool path_is_dir;

    };
}
