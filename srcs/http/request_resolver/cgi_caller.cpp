#include "cgi_caller.hpp"

namespace ws
{
    CGICaller::CGICaller(HttpRequest *request, EnvMap *env_map, Connection *connection)
        : request_(request), env_map_(env_map), connection_(connection){}

    RequestHandlerPayload *CGICaller::handle(RequestHandlerPayload *payload) 
    {
        std::string cgi_executable = this->get_cgi_executable(payload->file_path, payload->location);

        if (cgi_executable.size() == 0)
            return RequestHandler::handle(payload);

		CGI cgi(cgi_executable,
			    *this->env_map_,
			    payload->file_path,
			    *this->request_,
			    *this->connection_
			);

		payload->response.status_code = cgi.execute();
		payload->response.status_msg = cgi.get_status_msg();
		payload->response.headers = cgi.get_header_map();
		payload->response.body = cgi.get_body();

        return payload;
    }

    std::string CGICaller::get_uri_file_extension(const std::string &file_path)
    {
		size_t dot_pos = file_path.find_last_of('.');
		std::string file_extension = file_path.substr(dot_pos);

		return file_extension;
    }

    std::string CGICaller::get_cgi_executable(const std::string &file_path, const Location &location)
    {
		if (location.cgis.size() == 0)
			return "";

        std::string file_extension = this->get_uri_file_extension(file_path);
		std::vector<CGISettings>::const_iterator it = location.cgis.begin();
		std::vector<CGISettings>::const_iterator ite = location.cgis.end();

		while (it != ite)
		{
			if (it->extension == file_extension)
				return it->executable;
			it++;
		}
        
        return "";
    }
}
