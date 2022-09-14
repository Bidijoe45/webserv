#include <unistd.h>
#include <signal.h>
#include <cstdlib>
#include <string>
#include <iostream>

#include "exec_utils.hpp"

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

	static pid_t fork_worker(int *fd, const char *path, const char *arg, char *const *const envp)
	{
   		pid_t pid = fork();
		if (pid == 0)
		{
			dup2(fd[1], 1);
			close(fd[1]);
			execle(path, path, arg, (char *) NULL, envp);
            exit(1);
        }
		if (pid == -1)
			exit(3);
		return pid;
	}

	static pid_t fork_timer(unsigned int timeout, pid_t worker_pid)
	{
		pid_t pid = fork();
   		if (pid == 0)
		{
			sleep(timeout);
			exit(2);
		}
		if (pid == -1)
		{
			if (waitpid(worker_pid, NULL, 0) == -1)
				exit(4);
			exit(3);
		}
		return pid;
	}

	static std::string get_exec_output()
	{
		std::string	output;
		char		c;

		while (std::cin.get(c))
			output += c;

		return output;
	}

	static void kill_remaining_process(pid_t exited_pid, pid_t worker_pid, pid_t timer_pid, int kill_signal = SIGKILL)
	{
		if (exited_pid == worker_pid)
	        kill(timer_pid, SIGKILL);
		else if (exited_pid == timer_pid)
	        kill(worker_pid, kill_signal);
		else if (exited_pid == -1)
			exit(4);
	}

	static std::string check_status_errors(int status)
	{
		switch (WEXITSTATUS(status))
		{
			case EXECLE_ERROR:
				return "execle";
			case TIMEOUT_ERROR:
				return "cgi timed out";
			case FORK_ERROR:
				return "fork";
			case WAIT_ERROR:
				return "wait";
			case OTHER_ERROR:
				return "something went wrong";
			default:
				return "";
		}
	}

	std::string exec_with_timeout(const char *path, const char *arg, char *const *const envp, unsigned int timeout, int kill_signal = SIGKILL)
	{
		int fd[2];
		if (pipe(fd) == -1)
			throw std::runtime_error("cgi exec: pipe");

		pid_t intermediate_pid = fork();
		int status;
		if (intermediate_pid == 0)
		{
			close(fd[0]);
	   		pid_t worker_pid = fork_worker(fd, path, arg, envp);
			close(fd[1]);
	        pid_t timer_pid = fork_timer(timeout, worker_pid);

			pid_t exited_pid = wait(&status);
			kill_remaining_process(exited_pid, worker_pid, timer_pid, kill_signal);
			if (wait(NULL) == -1)
				exit(4);
			if (WIFEXITED(status) == false)
				exit(5);
			
	        exit(WEXITSTATUS(status));
	    }
		if (intermediate_pid == -1)
			throw std::runtime_error("cgi exec: fork");

		close(fd[1]);
	    if (waitpid(intermediate_pid, &status, 0) == -1)
			throw std::runtime_error("cgi exec: wait");

		if (WIFEXITED(status) == false)
			throw std::runtime_error("cgi exec: something went wrong");
		else if (WEXITSTATUS(status) != NO_ERROR)
			throw std::runtime_error("cgi exec: " + check_status_errors(status));

		int old_stdin = dup(0);
		dup2(fd[0], 0);
		close(fd[0]);

		std::string output = get_exec_output();

		dup2(old_stdin, 0);
		close(old_stdin);

		return output;
	}
}

