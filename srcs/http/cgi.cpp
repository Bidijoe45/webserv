#include <vector>
//#include <unistd.h>
#include <sstream>
#include <signal.h>

#include "http_request.hpp"
#include "cgi_settings.hpp"
#include "cgi.hpp"
#include "env_map.hpp"
#include "string_utils.hpp"
#include "http_headers.hpp"
#include "http_header.hpp"
#include "http_header_map.hpp"
#include "http_header_parser.hpp"
#include "exec_utils.hpp"

#include <iostream>

namespace ws
{

    CGI::CGI() {}

	void CGI::set_env(const EnvMap &env, const std::string &file_path, const HttpRequest &request)
	{
		std::cout << "SETTING CGI ENVS....." << std::endl;
		this->env_ = env;
		
		HttpHeaderMap::const_iterator it = request.headers.find("content-length");
		HttpHeaderMap::const_iterator ite = request.headers.end();
		if (it != ite)
		{
			HttpHeaderContentLength *content_len = static_cast<HttpHeaderContentLength *>(it->second);
			size_t body_size = content_len->content_length;
			this->env_.insert("CONTENT_LENGTH", ul_to_string(body_size)); 
		}
		it = request.headers.find("content-type");
		//TODO: implementar http header content-type y encontrar el que corresponde en lugar de meterlo a mano aqui:
		if (it != ite)
			this->env_.insert("CONTENT_TYPE", "application/octet-stream");

		this->env_.insert("GATEWAY_INTERFACE", "CGI/1.1");
		this->env_.insert("SCRIPT_NAME", file_path);
		this->env_.insert("SCRIPT_FILENAME", file_path);
		this->env_.insert("PATH_INFO", file_path);
		//TODO: coger query string de la request.uri. se setea aunque sea vacia ""
		this->env_.insert("QUERY_STRING", "hola=adios&que=tal");
		//TODO: guardar la IP del cliente:
		this->env_.insert("REMOTE_ADDR", "1.1.1.1");
		//TODO: remote host es un should, habria que encontrarlo del servidor tb, o dejar el remote_addr, o dejarlo en null:
		this->env_.insert("REMOTE_HOST", "");

		it = request.headers.find("host");
		HttpHeaderHost *host_header = static_cast<HttpHeaderHost *>(it->second);
		this->env_.insert("SERVER_NAME", host_header->host);
		//FIXME: default server port needs to be set even if it doesn't appear in the host header
		if (host_header->port != -1)
			this->env_.insert("SERVER_PORT", int_to_string(host_header->port));

		this->env_.insert("SERVER_PROTOCOL", "HTTP/1.1");
		this->env_.insert("SERVER_SOFTWARE", "webserv/1.0");

		this->env_.insert("REDIRECT_STATUS", "200");
	}

    unsigned int CGI::execute(const std::string &file_path)
    {
		std::cout << "EXECUTING " << this->executable_<< " " << file_path << ".........." << std::endl;

		Executer cgi_executer(this->executable_, file_path, this->env_);
		try
		{
			this->execution_output_ = cgi_executer.exec_with_timeout(5); 
		}
		catch (const std::runtime_error &e)
		{
			std::cout << e.what() << std::endl;
			return 500;
		}
		
		std::cout << "CGI RESPONSE: " << execution_output_ << std::endl;
		return 200;
    }

	void CGI::set_executable(const std::string &executable)
	{
		this->executable_ = executable;
	}

	const std::string CGI::get_executable() const
	{
		return this->executable_;
	}

	std::string CGI::get_cgi_header_name(std::string line)
	{
		std::string header_name;
		size_t colon_pos = line.find(':');

		if (colon_pos == std::string::npos)
			return "";

		header_name = line.substr(0, colon_pos); 
		header_name = string_to_lower(header_name, header_name.size());

		if (!is_token(header_name))
			return "";

		return header_name;
	}

	std::string CGI::get_cgi_header_value(std::string line)
	{
		std::string header_value;

		size_t value_start = line.find_first_not_of(" \t\n");
		if (value_start == std::string::npos)
			return "";
		size_t value_end = line.find_last_not_of(" \t");

		header_value = line.substr(value_start, value_end - value_start + 1);
		
		return header_value;
	}

	HttpHeaderMap CGI::parse_cgi_headers()
	{
		HttpHeaderMap headers;
		HttpHeader *parsed_header;
		std::string line;
		std::string header_name;
		std::string header_value;

		//if (this->execution_output_.size() == 0)
		//	return "";
		
		size_t pos_end_of_header_section = this->execution_output_.find("\r\n");
		size_t pos0 = 0;
		size_t pos1 = this->execution_output_.find('\n');

		while (pos1 != pos_end_of_header_section && pos1 != std::string::npos)
		{
			line = this->execution_output_.substr(pos0, pos1 - pos0);
			header_name = this->get_cgi_header_name(line);
			if (header_name == "")
			{
				pos0 = pos1 + 1;
				pos1 = this->execution_output_.find('\n', pos0);
				continue;
			}
			header_value = this->get_cgi_header_value(line.substr(line.find(':') + 1));
			headers.insert(header_name, parsed_header);
			pos0 = pos1 + 1;
			pos1 = this->execution_output_.find('\n', pos0);
		}

		return headers;
	}
    
	std::string CGI::parse_cgi_body()
	{
		size_t pos_end_of_header_section = this->execution_output_.find("\r\n"); // comprobar tipo de NL
		if (pos_end_of_header_section == std::string::npos)
			return "";
		std::string body = this->execution_output_.substr(pos_end_of_header_section);
		return body;
	}
}
