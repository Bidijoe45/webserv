#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>

#include "../utils/string_utils.hpp"
#include "http_request_resolver.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "http_uri.hpp"
#include "../settings/error_page.hpp"
#include "../settings/server_settings.hpp"
#include "../server/file_system.hpp"
#include "./headers/http_headers.hpp"
#include "location_resolver.hpp"
#include "../settings/cgi_settings.hpp"
#include "cgi.hpp"
#include "http_multipart_body_parser.hpp"
#include "http_multipart_body.hpp"
#include "http_header.hpp"
#include "../utils/env_map.hpp"
#include "connection.hpp"
#include "content_type_map.hpp"

namespace ws
{

    HttpRequestResolver::HttpRequestResolver(const HttpRequest &request, const ServerSettings &settings, const EnvMap &env, const Connection &connection, const ContentTypeMap &content_types)
    {
        this->request_ = request;
        this->settings_ = settings;
		this->env_ = env;
		this->connection_ = connection;
		this->content_types_ = content_types;
    }

	std::string HttpRequestResolver::resolve_status_code()
	{
		switch (this->response_.status_code)
		{
			case 200:
				return "OK";
			case 400:
				return "BAD REQUEST";
			case 404:
				return "NOT FOUND";
			case 403:
				return "FORBIDDEN";
			case 500:
				return "INTERNAL SERVER ERROR";
			case 501:
				return "NOT IMPLEMENTED";
			default:
				return "";
		}
	}

	void HttpRequestResolver::apply_method()
	{
		switch (this->request_.request_line.method)
		{
			case HTTP_METHOD_GET:
				this->apply_get_method();
				break;
			case HTTP_METHOD_POST:
				this->apply_post_method();
				break;
			case HTTP_METHOD_DELETE:
				this->apply_delete_method();
				break;
			default:
				this->response_.status_code = 501;
				break;
		}
	}

	std::string HttpRequestResolver::resolve_content_type(std::string extension)
	{
		if (extension.size() == 0)
			return "";

		ContentTypeMap::iterator type_it = this->content_types_.find(extension);
		if (type_it != this->content_types_.end())
			return type_it->second;

		return "";
	}

	void HttpRequestResolver::apply_get_method()
	{
		FileSystem file(this->file_path_);
		std::string content_type;
        
		if (!file.is_valid())
		{
			this->response_.status_code = 404;
			return;
		}

		if (file.is_dir())
		{
		    if (this->location_.index.size() > 0)
		    {
		        this->file_path_ = this->file_path_ + this->location_.index;
		        FileSystem new_file(this->file_path_);
		        if (!new_file.is_valid())
		        {
			        this->response_.status_code = 404; //Deberia devolver 404 o 403?
			        return;
		        }
		        this->response_.body = new_file.get_content();
		    }
		    else if (this->location_.autoindex == true)
		    {
		        this->response_.body = this->generate_autoindex(file);
		    }
		    else
		    {
		        this->response_.status_code = 403;
		        return;
		    }
			content_type = "text/html";
		}
		else
		{
		    this->response_.body = file.get_content();
			std::string file_extension = file.get_file_extension();
			content_type = resolve_content_type(file_extension);
			if (content_type.size() == 0)
				content_type = "application/octet-stream";
		}

		HttpHeaderContentLength *content_length_header = new HttpHeaderContentLength();
		content_length_header->set_value(this->response_.body.size());
		this->response_.headers.insert(content_length_header);

		HttpHeaderContentType *content_type_header = new HttpHeaderContentType();
		content_type_header->set_value(content_type);
		this->response_.headers.insert(content_type_header);

		this->response_.status_code = 200;
	}

	void HttpRequestResolver::apply_post_method()
	{
	    if (this->location_.upload_dir.size() == 0)
	    {
	        this->response_.status_code = 403;
	        return;
	    }

        HttpHeaderMap::iterator header_map_it = this->request_.headers.find("content-type");
        if (header_map_it == this->request_.headers.end())
        {
            this->response_.status_code = 400;
            return;
        }

	    HttpHeaderContentType *content_type = dynamic_cast<HttpHeaderContentType *>(header_map_it->second); 
	    if (content_type == NULL)
	    {
	        this->response_.status_code = 400;
	        return;
	    }

	    if (content_type->content_type != "multipart/form-data")
	    {
	        this->response_.status_code = 403;
	        return;
	    }

        std::map<std::string, std::string>::iterator boundary_param = content_type->parameters.find("boundary");
        if (boundary_param == content_type->parameters.end())
        {
	        this->response_.status_code = 400;
	        return;
        }

	    HttpMultipartBodyParser multipart_parser(this->request_.body, boundary_param->second);
	    HttpMultipartBody multipart_body = multipart_parser.parse();

        if (!multipart_parser.is_valid())
        {
            this->response_.status_code = 400;
            return;
        }

        std::vector<HttpMultipartBodyPart>::iterator it = multipart_body.parts.begin();
        std::vector<HttpMultipartBodyPart>::iterator ite = multipart_body.parts.end();

        while (it != ite)
        {
            HttpMultipartBodyPart part = *it;

            HttpHeaderMap::iterator header_cd_it = part.header_map.find("content-disposition");
            if (header_cd_it == part.header_map.end())
            {
                this->response_.status_code = 400;
                return;
            }

            HttpHeaderContentDisposition * header_cd = dynamic_cast<HttpHeaderContentDisposition *>(header_cd_it->second);
            if (header_cd == NULL)
            {
                this->response_.status_code = 400;
                return;
            }

            if (header_cd->filename.size() == 0)
            {
                it++;
                continue;
            }
        
            std::string file_path = this->location_.upload_dir + header_cd->filename;
            FileSystem file(file_path);

            if (!file.is_open())
                file.create(file_path);
            file.write(part.content);
            file.close();

            it++;
        }

        this->apply_get_method();
	}

	void HttpRequestResolver::apply_delete_method()
	{
	    if (this->location_.upload_dir.size() == 0)
	    {
	        this->response_.status_code = 403;
	        return;
	    }

	    FileSystem file(this->file_path_);
	    if (!file.is_valid() || file.is_dir())
	    {
	        this->response_.status_code = 403;
	        return ;
	    }
	
	    file.remove();
        file.close();
        HttpHeaderContentLength *hcl = new HttpHeaderContentLength();
        hcl->set_value(0);
        this->response_.headers.insert(hcl);
        this->response_.status_code = 200;
	}

    std::string HttpRequestResolver::generate_autoindex(const FileSystem &file)
    {
        std::string autoindex_html;
        std::vector<std::string> dir_files = file.read_dir();
            
        autoindex_html.append("<html>");
        autoindex_html.append("<head>");
        autoindex_html.append("<title>");
        autoindex_html.append("Index of " + this->request_.request_line.uri.path);
        autoindex_html.append("</title>");
        autoindex_html.append("</head>");
        autoindex_html.append("<body>");
        autoindex_html.append("<h1>");
        autoindex_html.append("Index of " + this->request_.request_line.uri.path);
        autoindex_html.append("</h1>");
        autoindex_html.append("<hr>");

        for (size_t i=0; i < dir_files.size(); i++)
        {
            autoindex_html.append("<a href=\"");
            autoindex_html.append(this->request_.request_line.uri.path);
            if (this->request_.request_line.uri.path.back() != '/')
                autoindex_html.append("/");
            autoindex_html.append(dir_files[i]);
            autoindex_html.append("\">");
            autoindex_html.append(dir_files[i]);
            autoindex_html.append("</a>");
            autoindex_html.append("<br/>");
        }

        autoindex_html.append("</hr>");
        autoindex_html.append("</body>");
        autoindex_html.append("</html>");

        return autoindex_html;
    }

	std::string HttpRequestResolver::find_error_page()
	{
		ServerSettings::error_pages_it it = this->settings_.error_pages.begin();
		ServerSettings::error_pages_it ite = this->settings_.error_pages.end();

		while (it != ite)
		{
			if (it->code == this->response_.status_code)
				return it->path;
			it++;
		}
		return "";
	}

	std::string HttpRequestResolver::create_default_error_page()
	{
		std::string error_page;
		std::string error = ul_to_string(this->response_.status_code) + " " + this->response_.status_msg;

		error_page.append("<!DOCTYPE html><html lang=\"en\"><head><title>");
		error_page.append(error);
		error_page.append("</title></head><body><h1>");
		error_page.append(error);
		error_page.append("</h1><p>Oops!</p><img src=\"https://http.cat/" + ul_to_string(this->response_.status_code) + "\"></body></html>");

		return error_page;
	}

	std::string HttpRequestResolver::resolve_custom_error_page(const std::string error_page_path)
	{
		FileSystem file(error_page_path);

		if (!file.is_valid())
			return this->create_default_error_page();

		return file.get_content();
	}

	void HttpRequestResolver::set_error_body()
	{
		std::string error_page_path = this->find_error_page();

		if (error_page_path == "")
			this->response_.body = this->create_default_error_page();
		else
			this->response_.body = resolve_custom_error_page(error_page_path);

		HttpHeaderMap::iterator content_length = this->response_.headers.find("content-length");
		if (content_length != this->response_.headers.end())
			this->response_.headers.erase(content_length);

		HttpHeaderContentLength *content_length_header = new HttpHeaderContentLength();
		content_length_header->set_value(this->response_.body.size());
		this->response_.headers.insert(content_length_header);
	}
	
	std::string HttpRequestResolver::resolve_cgi_executable()
	{
		if (this->location_.cgis.size() == 0)
			return "";
		size_t dot_pos = this->file_path_.find_last_of('.');
		std::string file_extension = this->file_path_.substr(dot_pos);
		std::vector<CGISettings>::iterator it = this->location_.cgis.begin();
		std::vector<CGISettings>::iterator ite = this->location_.cgis.end();
		while (it != ite)
		{
			if (it->extension == file_extension)
				return it->executable;
			it++;
		}
		return "";
	}

    HttpResponse HttpRequestResolver::resolve()
    {
		this->response_.http_version = "HTTP/1.1";

		if (this->request_.is_valid == false)
		{
			this->response_.status_code = 400;
		}
		else
		{
			LocationResolver location_resolver = LocationResolver(this->settings_.locations);
            std::string uri_path = this->request_.request_line.uri.path;
			this->location_ = location_resolver.resolve(this->request_.request_line.uri);

            if (this->location_.path.size() == 0)
            {
                this->response_.status_code = 404;
            }
            else if (this->location_.redirect.code > 0)
		    {
		        this->response_.status_code = this->location_.redirect.code;

                HttpHeaderLocation *location_header = new HttpHeaderLocation();
                location_header->uri = this->location_.redirect.to;
                location_header->value = location_header->uri.absolute_path();

		        this->response_.headers.insert(location_header);
		    }
            else
            {
                std::string new_uri_path; 
                if (uri_path.compare(0, this->location_.path.size(), this->location_.path) == 0)
                    new_uri_path = this->request_.request_line.uri.path.substr(this->location_.path.size());
			    this->file_path_ = this->location_.root + new_uri_path;
				std::string cgi_executable = this->resolve_cgi_executable();
				if (cgi_executable != "")
				{
					CGI cgi(cgi_executable, this->env_, this->file_path_, this->request_, this->connection_);
					this->response_.status_code = cgi.execute();
					this->response_.status_msg = cgi.get_status_msg();
					this->response_.headers = cgi.get_header_map();
					this->response_.body = cgi.get_body();
				}
				else
					this->apply_method();
            }
		}

		if (this->response_.status_msg.size() == 0)
			this->response_.status_msg = this->resolve_status_code();

		if (this->response_.status_code >= 300)
			this->set_error_body();

        return this->response_;
    }

}
