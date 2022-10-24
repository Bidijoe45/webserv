#include <vector>
#include <string>
#include <iostream>

#include "content_type_map.hpp"
#include "string_utils.hpp"
#include "file_system.hpp"

namespace ws
{
	ContentTypeMap::ContentTypeMap() : valid_(false) {}

	ContentTypeMap::ContentTypeMap(const std::string &content_types_file_path) : valid_(false)
	{
		this->parse_content_types_file(content_types_file_path);
	}

	ContentTypeMap::ContentTypeMap(const ContentTypeMap &src) : map_(src.map_), valid_(src.valid_) {}

	ContentTypeMap &ContentTypeMap::operator=(const ContentTypeMap &src)
	{
		if (this != &src)
		{
			this->map_ = src.map_;
			this->valid_ = src.valid_;
		}
		return *this;
	}

	void ContentTypeMap::parse_content_types_file(const std::string &content_types_file_path)
	{
		FileSystem file(content_types_file_path);
		if (file.is_open() == false)
			return;

		std::string file_content = file.get_content();
		if (file_content.size() == 0)
			return;

		std::vector<std::string> lines = string_split(file_content, "\n");

		std::vector<std::string>::const_iterator it = lines.begin();
		std::vector<std::string>::const_iterator ite = lines.end();
		while (it != ite)
		{
			std::vector<std::string> line = string_split(*it, " ");
			if (line.size() != 2)
				return;
			this->insert(line[0], line[1]);
			it++;
		}

		this->valid_ = true;
	}

	ContentTypeMap::iterator ContentTypeMap::begin()
	{
		return this->map_.begin();
	}

	ContentTypeMap::const_iterator ContentTypeMap::begin() const
	{
		return this->map_.begin();
	}

	ContentTypeMap::iterator ContentTypeMap::end()
	{
		return this->map_.end();
	}

	ContentTypeMap::const_iterator ContentTypeMap::end() const
	{
		return this->map_.end();
	}

	std::pair<ContentTypeMap::iterator, bool> ContentTypeMap::insert(std::string extension, std::string type)
	{
		return this->map_.insert(std::make_pair<std::string, std::string>(extension, type));
	}

	bool ContentTypeMap::is_valid()
	{
		return this->valid_;
	}
	
	ContentTypeMap::iterator ContentTypeMap::find(const std::string &k)
	{
		return this->map_.find(k);
	}

	ContentTypeMap::const_iterator ContentTypeMap::find(const std::string &k) const
	{
		return this->map_.find(k);
	}

}

