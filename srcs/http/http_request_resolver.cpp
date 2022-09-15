#include <fstream>
#include <iostream>

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
#include "http_multipart_body_parser.hpp"
#include "http_multipart_body.hpp"

namespace ws
{

    HttpRequestResolver::HttpRequestResolver(const HttpRequest &request, const ServerSettings &settings)
    {
        this->request_ = request;
        this->settings_ = settings;
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
			default:
				return "";
		}
	}

	void HttpRequestResolver::apply_method()
	{
		switch (this->request_.method)
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

	void HttpRequestResolver::apply_get_method()
	{
		FileSystem file(this->file_path_);
        
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
		        return ;
		    }
		}
		else
		    this->response_.body = file.get_content();

		HttpHeaderContentLength *content_length_header = new HttpHeaderContentLength();
		content_length_header->set_value(this->response_.body.size());
		this->response_.headers.insert(content_length_header);

		//TODO: agregar el tipo del archivo tambien

		this->response_.status_code = 200;
	}

	void HttpRequestResolver::apply_post_method()
	{
	    this->response_.status_code = 100;
	    return ;

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

        // std::cout << "XXXXXXXXXXXXXXXXXX" << std::endl;
        // std::cout << content_type->parameters.size() << std::endl;
        // std::map<std::string, std::string>::iterator it = content_type->parameters.begin();
        // std::map<std::string, std::string>::iterator ite = content_type->parameters.end();
        //
        // while (it != ite)
        // {
        //     std::cout << it->first << " : " << it->second << std::endl;
        //     it++;
        // }

        // std::cout << content_type->content_type << std::endl;
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

        std::cout << "END apply post" << std::endl;
	}

	void HttpRequestResolver::apply_delete_method()
	{

	}

    std::string HttpRequestResolver::generate_autoindex(const FileSystem &file)
    {
        std::string autoindex_html;
        std::vector<std::string> dir_files = file.read_dir();
            
        autoindex_html.append("<html>");
        autoindex_html.append("<head>");
        autoindex_html.append("<title>");
        autoindex_html.append("Index of " + this->request_.uri.path);
        autoindex_html.append("</title>");
        autoindex_html.append("</head>");
        autoindex_html.append("<body>");
        autoindex_html.append("<h1>");
        autoindex_html.append("Index of " + this->request_.uri.path);
        autoindex_html.append("</h1>");
        autoindex_html.append("<hr>");

        for (size_t i=0; i < dir_files.size(); i++)
        {
            autoindex_html.append("<a href=\"");
            autoindex_html.append(this->request_.uri.path);
            if (this->request_.uri.path.back() != '/')
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
		std::string error = int_to_string(this->response_.status_code) + " " + this->response_.status_msg;

		error_page.append("<!DOCTYPE html><html lang=\"en\"><head><title>");
		error_page.append(error);
		error_page.append("</title></head><body><h1>");
		error_page.append(error);
		error_page.append("</h1><p>Oops!</p><img src=\"https://media2.giphy.com/media/C23cMUqoZdqMg/giphy.gif?cid=ecf05e47mrq894fkcolgk88zpywmc0gafc8631nzd474dt8m&rid=giphy.gif&ct=g\"></body></html>");

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

		HttpHeaderContentLength *content_length_header = new HttpHeaderContentLength();
		content_length_header->set_value(this->response_.body.size());
		this->response_.headers.insert(content_length_header);
	}

    HttpResponse HttpRequestResolver::resolve()
    {
		this->response_.http_version = "HTTP/1.1";

		if (this->request_.is_valid() == false)
		{
			this->response_.status_code = 400;
		}
		else
		{
			LocationResolver location_resolver = LocationResolver(this->settings_.locations);
            std::string uri_path = this->request_.uri.path;
			this->location_ = location_resolver.resolve(this->request_.uri);

            if (this->location_.path.size() == 0)
                this->response_.status_code = 404;
            else
            {
                std::string new_uri_path; 
                if (uri_path.compare(0, this->location_.path.size(), this->location_.path) == 0)
                    new_uri_path = this->request_.uri.path.substr(this->location_.path.size());
			    this->file_path_ = this->location_.root + new_uri_path;
			    this->apply_method();
            }
		}

		this->response_.status_msg = this->resolve_status_code();

		if (this->response_.status_code >= 400)
			this->set_error_body();

        return this->response_;
    }
    
    }
