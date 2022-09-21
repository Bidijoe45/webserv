#pragma once

#include <unistd.h>
#include <cstdlib>
#include <string>

#include "env_map.hpp"

namespace ws
{
	enum EXEC_ERROR
	{
		NO_ERROR,
		PIPE_ERROR,
		EXECLE_ERROR,
		TIMEOUT_ERROR,
		FORK_ERROR,
		WAIT_ERROR,
		DUP_ERROR,
		READ_ERROR,
		OTHER_ERROR
	};

	class Executer
	{
		public:
			Executer(const std::string &path, const std::string &arg, EnvMap env, const std::string &body);
			~Executer();
			std::string exec_with_timeout(unsigned int timeout, int kill_signal = SIGKILL);

		private:
			pid_t fork_worker(int *fd);
			pid_t fork_timer(unsigned int timeout);
			std::string get_exec_output(int fd);
			void kill_remaining_process(pid_t exited_pid, int kill_signal = SIGKILL);
			std::string get_error_string(EXEC_ERROR error);
			void delete_double_pointer();

			const std::string	path_;
			const std::string	arg_;
			const std::string	body_;
			char 				**envp_;
			pid_t				worker_pid_;
			pid_t				timer_pid_;
	};
}

