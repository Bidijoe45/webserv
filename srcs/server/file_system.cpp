#include <sys/stat.h>
#include <dirent.h>
#include <iostream>

#include "file_system.hpp"

namespace ws
{
    FileSystem::FileSystem() : is_valid_(false) {}

    FileSystem::FileSystem(const std::string &file_path) : is_valid_(false)
    {
        this->open(file_path);
    }

    FileSystem::~FileSystem()
    {
        if (this->is_open())
            this->file_.close();
    }

    void FileSystem::open(const std::string &file_path)
    {
        this->path_ = file_path;

        std::cout << this->path_ << std::endl;

        if (lstat(this->path_.c_str(), &this->stat_) != 0)
            return;

        if (S_ISDIR(this->stat_.st_mode))
        {
            this->path_is_dir_ = true;
            this->is_valid_ = true;
            return;
        }

        this->file_.open(this->path_.c_str());
        this->is_valid_ = true;
    }

    bool FileSystem::is_open() const
    {
        return this->file_.is_open();
    }

    bool FileSystem::is_dir() const
    {
        return this->path_is_dir_;
    }

    bool FileSystem::is_valid() const
    {
        return this->is_valid_;
    }

    std::string FileSystem::get_content() 
    {
		return std::string((std::istreambuf_iterator<char>(this->file_)), std::istreambuf_iterator<char>());
    }

    std::string FileSystem::get_path() const
    {
        return this->path_;
    }

    std::vector<std::string> FileSystem::read_dir() const
    {
        std::vector<std::string> dir_files;
        DIR *dir_fd;
        struct dirent* dir_file;
        
        if ((dir_fd = opendir(this->path_.c_str())) == NULL) 
            return std::vector<std::string>();

        while ((dir_file = readdir(dir_fd))) 
        {
            if (!strcmp (dir_file->d_name, "."))
                continue;
            if (dir_file->d_type == DT_DIR)
                dir_files.push_back(std::string(dir_file->d_name) + "/");
            else
                dir_files.push_back(std::string(dir_file->d_name));
        }

        return dir_files;
    }
}
