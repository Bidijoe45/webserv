#pragma once

#include <map>
#include <string>
#include <utility>

namespace ws
{
	struct EnvMap
	{
		public:
			typedef std::map<std::string, std::string>::iterator iterator;
			typedef std::map<std::string, std::string>::const_iterator const_iterator;

			EnvMap();
			EnvMap(char **env);
			void set_from(char **env);
			iterator begin();
			const_iterator begin() const;
			iterator end();
			const_iterator end() const;
			std::pair<iterator,bool> insert(std::string name, std::string value);
		
		private:
			std::map<std::string, std::string> env_;
	};
}
