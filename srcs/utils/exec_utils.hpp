#pragma once

#include <unistd.h>
#include <cstdlib>
#include <string>

namespace ws
{
	enum EXEC_ERROR
	{
		NO_ERROR,
		EXECLE_ERROR,
		TIMEOUT_ERROR,
		FORK_ERROR,
		WAIT_ERROR,
		OTHER_ERROR
	};

	class Executer
	{
		public:
			Executer(const std::string &path, const std::string &arg, char **envp);
			Executer();
			~Executer();
			std::string exec_with_timeout(unsigned int timeout, int kill_signal);

		private:
			pid_t fork_worker(int *fd);
			pid_t fork_timer(unsigned int timeout);
			std::string get_exec_output();
			void kill_remaining_process(pid_t exited_pid, int kill_signal);
			std::string check_status_errors(int status);
			void delete_double_pointer();

			const std::string	path_;
			const std::string	arg_;
			char 				**envp_;
			pid_t				worker_pid_;
			pid_t				timer_pid_;
	};
}

