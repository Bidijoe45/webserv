#include <string>
#include <utility>
#include <cstdlib>
#include "env_map.hpp"

namespace ws
{
	EnvMap::EnvMap() {}

	EnvMap::EnvMap(char **env)
	{
		this->set_from(env);	
	}

	static std::string read_env_name(char *env_var)
	{
		size_t n = 0;

		while (env_var[n] != '\0' && env_var[n] != '=')
			n++;
		return std::string(env_var, n);
	}

	void EnvMap::set_from(char **env)
	{
		std::string name;
		std::string value;

		for (size_t i = 0; env[i] != NULL; i++)
		{
			name = read_env_name(env[i]);
			value = getenv(name.c_str());
			this->insert(name, value);
		}
	}

	EnvMap::iterator EnvMap::begin()
	{
		return this->env_.begin();
	}

	EnvMap::const_iterator EnvMap::begin() const
	{
		return this->env_.begin();
	}

	EnvMap::iterator EnvMap::end()
	{
		return this->env_.end();
	}

	EnvMap::const_iterator EnvMap::end() const
	{
		return this->env_.end();
	}

	std::pair<EnvMap::iterator, bool> EnvMap::insert(std::string name, std::string value)
	{
		return this->env_.insert(std::make_pair<std::string, std::string>(name, value));
	}
}

