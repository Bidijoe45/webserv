#pragma once

#include <sys/stat.h>
#include <string>
#include <fstream>

namespace ws
{
    class FileSystem
    {
        public:
            FileSystem();
            FileSystem(const std::string &file_path);
            ~FileSystem();
            void open(const std::string &file_path);
            bool is_valid();
            bool is_open();
            bool is_dir();
            std::string get_path();
            std::string get_content();
        
        private:
            std::string path_;
            bool path_is_dir_;
            bool is_valid_;
            std::ifstream file_;
            struct stat stat_;

    };
}
