#pragma once

#include <map>
#include <string>

namespace ws
{
	class ContentTypeMap
	{
		public:
			typedef std::map<std::string, std::string>::iterator iterator;
			typedef std::map<std::string, std::string>::const_iterator const_iterator;

			ContentTypeMap();
			ContentTypeMap(const std::string &content_types_file_path);
			ContentTypeMap(const ContentTypeMap &src);
			ContentTypeMap &operator=(const ContentTypeMap &src);

			void parse_content_types_file(const std::string &content_types_file_path);
			iterator begin();
			const_iterator begin() const;
			iterator end();
			const_iterator end() const;
			std::pair<iterator,bool> insert(std::string extension, std::string type);
			bool is_valid();
			iterator find(const std::string &k);
			const_iterator find(const std::string &k) const;

		private:
			std::map<std::string, std::string> map_;
			bool valid_;
	};
}
