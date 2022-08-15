#include <sys/stat.h>

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

    bool FileSystem::is_open()
    {
        return this->file_.is_open();
    }

    bool FileSystem::is_dir()
    {
        return this->path_is_dir_;
    }

    bool FileSystem::is_valid()
    {
        return this->is_valid_;
    }

    std::string FileSystem::get_content()
    {
		return std::string((std::istreambuf_iterator<char>(this->file_)), std::istreambuf_iterator<char>());
    }

}
