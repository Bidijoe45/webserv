#include <vector>
#include <unistd.h>
#include <sstream>

#include "http_request.hpp"
#include "cgi_settings.hpp"
#include "cgi.hpp"
#include "env_map.hpp"

#include <iostream>

namespace ws
{

    CGI::CGI() {}

	void CGI::set_env(const EnvMap &env, const std::string &file_path, const HttpRequest &request)
	{
		std::cout << "SETTING CGI ENVS....." << std::endl;
		this->env_ = env;
		this->env_.insert("REQUEST_METHOD", request.method_to_string());	
	}

    void CGI::execute(const std::string &file_path)
    {
		std::cout << "EXECUTING " << this->executable_<< " " << file_path << ".........." << std::endl;
		char **envp = this->env_.get_double_pointer();

		int fd[2];
		pipe(fd);

		pid_t pid = fork();
		int status;
		if (pid == 0)
		{
			close(fd[0]);
			dup2(fd[1], 1);
			close(fd[1]);
			execle(this->executable_.c_str(), this->executable_.c_str(), file_path.c_str(), (char *) NULL, envp);
		}
		if (pid == -1)
			std::cout << "FORK ERROR" << std::endl;

		close(fd[1]);
		waitpid(pid, &status, 0);

		this->delete_double_pointer(envp);	

		int old_stdin = dup(0);
		dup2(fd[0], 0);
		close(fd[0]);

		std::string exec_output;
		char c;
		while (std::cin.get(c))
			exec_output += c;

		dup2(old_stdin, 0);
		close(old_stdin);

		//system("lsof -c webserv");
		std::cout << "CGI RESPONSE: " << exec_output << std::endl;

    }

	void CGI::delete_double_pointer(char **envp)
	{
		for (size_t i = 0; envp[i] != NULL; i++)
			delete[] envp[i];
		delete[] envp;
	}
	
	void CGI::set_executable(const std::string &executable)
	{
		this->executable_ = executable;
	}

	const std::string CGI::get_executable() const
	{
		return this->executable_;
	}
    
}
