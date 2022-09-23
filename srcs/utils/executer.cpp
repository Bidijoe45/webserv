#include <unistd.h>
#include <signal.h>
#include <cstdlib>
#include <string>

#include "executer.hpp"
#include "env_map.hpp"

namespace ws
{
	Executer::Executer(const std::string &path, const std::string &arg, EnvMap env, const std::string &body) : path_(path), arg_(arg), body_(body)
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
		int fd_body[2];
		if (pipe(fd_body) == -1)
			exit(PIPE_ERROR);
		write(fd_body[1], this->body_.c_str(), this->body_.size());
		close(fd_body[1]);
   		pid_t pid = fork();
		if (pid == 0)
		{
			if (dup2(fd_body[0], STDIN_FILENO) == -1)
				exit(DUP_ERROR);
			close(fd_body[0]);
			if (dup2(fd[1], STDOUT_FILENO) == -1)
				exit(DUP_ERROR);
			close(fd[1]);
			execle(this->path_.c_str(), this->path_.c_str(), this->arg_.c_str(), (char *) NULL, this->envp_);
			exit(EXECLE_ERROR);
        }
		if (pid == -1)
			exit(FORK_ERROR);
		close(fd_body[0]);
		return pid;
	}

	pid_t Executer::fork_timer(unsigned int timeout)
	{
		pid_t pid = fork();
   		if (pid == 0)
		{
			sleep(timeout);
			this->delete_double_pointer();
			exit(TIMEOUT_ERROR);
		}
		if (pid == -1)
		{
			if (waitpid(this->worker_pid_, NULL, 0) == -1)
				exit(WAIT_ERROR);
			exit(FORK_ERROR);
		}
		return pid;
	}

	std::string Executer::get_exec_output(int fd)
	{
		std::string	output;
		size_t	ret;
		char	buff[64];
		while ((ret = read(fd, buff, 64)) > 0)
			output.append(buff, ret);
		if (ret == -1)
			throw std::runtime_error(get_error_string(READ_ERROR));
		return output;
	}

	void Executer::kill_remaining_process(pid_t exited_pid, int kill_signal)
	{
		if (exited_pid == this->worker_pid_)
	        kill(this->timer_pid_, kill_signal);
		else if (exited_pid == this->timer_pid_)
	        kill(this->worker_pid_, kill_signal);
		else if (exited_pid == -1)
			exit(WAIT_ERROR);
	}

	std::string Executer::get_error_string(EXEC_ERROR error)
	{
		std::string str = "cgi exec: ";
		switch (error)
		{
			case PIPE_ERROR:
				return str + "pipe";
			case EXECLE_ERROR:
				return str + "execle";
			case TIMEOUT_ERROR:
				return str + "cgi timed out";
			case FORK_ERROR:
				return str + "fork";
			case WAIT_ERROR:
				return str + "wait";
			case DUP_ERROR:
				return str + "dup";
			case READ_ERROR:
				return str + "read";
			default:
				return str + "something went wrong";
		}
	}

	void Executer::check_post_exec_errors(pid_t intermediate_pid, int *fd)
	{
		int status;

		if (waitpid(intermediate_pid, &status, 0) == -1)
		{
			close(fd[0]);
			throw std::runtime_error(this->get_error_string(WAIT_ERROR));
		}
		if (WIFEXITED(status) == false)
		{
			close(fd[0]);
			throw std::runtime_error(this->get_error_string(OTHER_ERROR));
		}
		EXEC_ERROR error = static_cast<EXEC_ERROR>(WEXITSTATUS(status));
		if (error > NO_ERROR && error <= OTHER_ERROR)
		{
			close(fd[0]);
			throw std::runtime_error(this->get_error_string(error));
		}
	}

	std::string Executer::exec_with_timeout(unsigned int timeout, int kill_signal)
	{
		int fd[2];
		if (pipe(fd) == -1)
			throw std::runtime_error(this->get_error_string(PIPE_ERROR));

		pid_t intermediate_pid = fork();
		if (intermediate_pid == 0)
		{
			close(fd[0]);
	   		this->worker_pid_ = this->fork_worker(fd);
			close(fd[1]);
			this->timer_pid_ = this->fork_timer(timeout);

			int status;
			pid_t exited_pid = wait(&status);
			kill_remaining_process(exited_pid, kill_signal);
			if (wait(NULL) == -1)
				exit(WAIT_ERROR);
			if (WIFEXITED(status) == false)
				exit(OTHER_ERROR);
			
			this->delete_double_pointer();
	        exit(WEXITSTATUS(status));
	    }
		if (intermediate_pid == -1)
			throw std::runtime_error(this->get_error_string(FORK_ERROR));

		close(fd[1]);
		this->check_post_exec_errors(intermediate_pid, fd);
	   	std::string output = get_exec_output(fd[0]);
		close(fd[0]);

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

