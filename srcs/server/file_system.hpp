#pragma once

#include <sys/stat.h>
#include <string>
#include <vector>
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
            bool is_valid() const;
            bool is_open() const;
            bool is_dir() const;
            std::string get_path() const;
            std::string get_content();
            std::vector<std::string> read_dir() const;
        
        private:
            std::string path_;
            bool path_is_dir_;
            bool is_valid_;
            std::ifstream file_;
            struct stat stat_;

    };
}
