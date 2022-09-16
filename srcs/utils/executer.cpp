#include <unistd.h>
#include <signal.h>
#include <cstdlib>
#include <string>
#include <iostream>

#include "executer.hpp"
#include "env_map.hpp"

namespace ws
{
	Executer::Executer(const std::string &path, const std::string &arg, EnvMap env) : path_(path), arg_(arg)
	{
		this->envp_ = env.get_double_pointer();
		this->worker_pid_ = -1;
		this->timer_pid_ = -1;
	}

	Executer::~Executer()
	{
		this->delete_double_pointer();
	}

	pid_t Executer::fork_worker(int *fd)
	{
   		pid_t pid = fork();
		if (pid == 0)
		{
			if (dup2(fd[1], 1) == -1)
				exit(4);
			close(fd[1]);
			execle(this->path_.c_str(), this->path_.c_str(), this->arg_.c_str(), (char *) NULL, this->envp_);
            exit(1);
        }
		if (pid == -1)
			exit(3);
		return pid;
	}

	pid_t Executer::fork_timer(unsigned int timeout)
	{
		pid_t pid = fork();
   		if (pid == 0)
		{
			sleep(timeout);
			exit(2);
		}
		if (pid == -1)
		{
			if (waitpid(this->worker_pid_, NULL, 0) == -1)
				exit(4);
			exit(3);
		}
		return pid;
	}

	std::string Executer::get_exec_output()
	{
		std::string	output;
		char		c;

		while (std::cin.get(c))
			output += c;

		return output;
	}

	void Executer::kill_remaining_process(pid_t exited_pid, int kill_signal)
	{
		if (exited_pid == worker_pid_)
	        kill(timer_pid_, SIGKILL);
		else if (exited_pid == timer_pid_)
	        kill(worker_pid_, kill_signal);
		else if (exited_pid == -1)
			exit(4);
	}

	std::string Executer::check_status_errors(int status)
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

	std::string Executer::exec_with_timeout(unsigned int timeout, int kill_signal)
	{
		int fd[2];
		if (pipe(fd) == -1)
			throw std::runtime_error("cgi exec: pipe");

		pid_t intermediate_pid = fork();
		int status;
		if (intermediate_pid == 0)
		{
			close(fd[0]);
	   		this->worker_pid_ = this->fork_worker(fd);
			close(fd[1]);
			this->timer_pid_ = this->fork_timer(timeout);

			pid_t exited_pid = wait(&status);
			kill_remaining_process(exited_pid, kill_signal);
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
		if (old_stdin == -1)
			throw std::runtime_error("cgi exec: dup");
		if (dup2(fd[0], 0) == -1)
			throw std::runtime_error("cgi exec: dup2");

		close(fd[0]);

		std::string output = get_exec_output();

		if (dup2(old_stdin, 0) == -1)
			throw std::runtime_error("cgi exec: dup2");
		close(old_stdin);
		//system("lsof -c webserv");
		return output;
	}

	void Executer::delete_double_pointer()
	{
		if (!this->envp_)
			return;

		for (size_t i = 0; this->envp_[i] != NULL; i++)
			delete[] this->envp_[i];
		delete[] this->envp_;
	}
}

