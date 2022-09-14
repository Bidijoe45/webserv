#pragma once

#include <string>

namespace ws
{
	std::string exec_with_timeout(const char *path, const char *arg, char *const *const envp, unsigned int timeout, int kill_signal);
}

