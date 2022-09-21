#include <vector>
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
#include "executer.hpp"
#include "data_buffer.hpp"

#include <iostream>

namespace ws
{

    CGI::CGI(const std::string &executable, const EnvMap &env, const std::string &file_path, const HttpRequest &request)
   	{
		this->executable_ = executable;
		this->env_ = env;
		this->file_path_ = file_path;
		this->request_ = request;
		this->set_env();
	}

	void CGI::set_env()
	{
		this->env_.insert("CONTENT_LENGTH", ul_to_string(this->request_.body.size())); 

		HttpHeaderMap::const_iterator it = this->request_.headers.find("content-type");
		HttpHeaderMap::const_iterator ite = this->request_.headers.end();
		if (it != ite)
			this->env_.insert("CONTENT_TYPE", it->second->value);

		this->env_.insert("GATEWAY_INTERFACE", "CGI/1.1");
		this->env_.insert("SCRIPT_NAME", this->file_path_);
		this->env_.insert("SCRIPT_FILENAME", this->file_path_);
		this->env_.insert("PATH_INFO", this->file_path_);
		this->env_.insert("QUERY_STRING", this->request_.request_line.uri.query);
		//TODO: guardar la IP del cliente:
		this->env_.insert("REMOTE_ADDR", "1.1.1.1");
		//TODO: remote host es un should, habria que encontrarlo del servidor tb, o dejar el remote_addr, o dejarlo en null:
		this->env_.insert("REMOTE_HOST", "");
		this->env_.insert("REQUEST_METHOD", this->request_.request_line.method_to_string());	

		it = this->request_.headers.find("host");
		HttpHeaderHost *host_header = static_cast<HttpHeaderHost *>(it->second);
		this->env_.insert("SERVER_NAME", host_header->host);
		//FIXME: default server port needs to be set even if it doesn't appear in the host header
		if (host_header->port != -1)
			this->env_.insert("SERVER_PORT", int_to_string(host_header->port));

		this->env_.insert("SERVER_PROTOCOL", "HTTP/1.1");
		this->env_.insert("SERVER_SOFTWARE", "webserv/1.0");

		this->env_.insert("REDIRECT_STATUS", "200");
	}

    unsigned int CGI::execute()
    {
		unsigned int status_code = 200;

		Executer cgi_executer(this->executable_, this->file_path_, this->env_, this->request_.body);
		try
		{
			this->execution_output_ = cgi_executer.exec_with_timeout(5); 
			if (this->execution_output_.size() == 0)
				throw std::runtime_error("CGI: no output from cgi");
			this->output_buff_ = DataBuffer(this->execution_output_);
			this->parse_execution_output();
		}
		catch (const std::runtime_error &e)
		{
			std::cout << e.what() << std::endl;
			return 500;
		}
	
		HttpHeaderMap::iterator status = this->response_headers_.find("status");
		if (status != this->response_headers_.end())
		{
			this->status_msg_ = static_cast<HttpHeaderCGIStatus*>(status->second)->reason_phrase;
			status_code = static_cast<HttpHeaderCGIStatus*>(status->second)->status_code;
			this->response_headers_.erase(status);
		}
		
		return status_code;
    }
			
	void CGI::parse_execution_output()
	{
		this->response_headers_ = this->parse_headers();
		if (this->response_headers_.find(HttpHeader::header_type_to_string(HTTP_HEADER_CONTENT_TYPE))!= this->response_headers_.end())
		{
			this->response_body_ = this->parse_body();
			HttpHeaderContentLength *content_length_header = new HttpHeaderContentLength(); 
			content_length_header->set_value(this->response_body_.size());
			this->response_headers_.insert(content_length_header);
		}
	}

	HttpHeaderMap CGI::parse_headers()
	{
		std::vector<std::string> header_block;
		HttpHeaderMap headers;
		std::string line = this->output_buff_.get_next_line("\n");
		if (line.back() == '\r')
			line.erase(line.size() - 1);

		while (line.size() != 0)
		{
			header_block.push_back(line);
			line = this->output_buff_.get_next_line("\n");
			if (line.back() == '\r')
				line.erase(line.size() - 1);
		}

		HttpHeaderParser header_parser(header_block);
		headers = header_parser.parse_block();

		if (!header_parser.is_valid())
			throw std::runtime_error("CGI: Invalid response header block");

		return headers;
	}
    
	std::string CGI::parse_body()
	{
		std::string body = this->output_buff_.flush(this->output_buff_.size());
		return body;
	}

	HttpHeaderMap CGI::get_header_map()
	{
		return this->response_headers_;
	}

	std::string CGI::get_body()
	{
		return this->response_body_;
	}

	std::string CGI::get_status_msg()
	{
		return this->status_msg_;
	}
}
