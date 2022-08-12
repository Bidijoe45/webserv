#include <fstream>
#include <iostream>

#include "http_request_resolver.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "http_uri.hpp" 

namespace ws
{

    HttpRequestResolver::HttpRequestResolver(const HttpRequest &request, const ServerSettings &settings)
    {
        this->request_ = request;
        this->settings_ = settings;
    }

    Location HttpRequestResolver::resolve_location()
    {
        Location location;
        int last_score;
        HttpUri request_uri = this->request_.uri;
        ServerSettings::locations_cit location_it = this->settings_.locations.begin(); 
        ServerSettings::locations_cit location_ite = this->settings_.locations.end(); 

        location = *location_it;
        last_score = request_.uri.path.compare((*location_it).path);
        location_it++;

        for (; location_it != location_ite; location_it++)
        {
            int score = request_.uri.path.compare((*location_it).path);
            if (score >= last_score)
                location = *location_it;
        }

        return location;
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
		std::cout << "File path: " << this->file_path_ << std::endl;
		std::ifstream in(this->file_path_);

		if (!in.is_open())
		{
			this->response_.status_code = 404;
			return;
		}

		std::string file_content((std::istreambuf_iterator<char>(in)), 
    		std::istreambuf_iterator<char>());

		this->response_.body = file_content;

		HttpHeaderContentLength *content_length_header = new HttpHeaderContentLength();
		content_length_header->set_value(file_content.size());
		this->response_.headers.insert(content_length_header);

		//TODO: agregar el tipo del archivo tambien

		this->response_.status_code = 200;
	}

	void HttpRequestResolver::apply_post_method()
	{

	}

	void HttpRequestResolver::apply_delete_method()
	{

	}

    HttpResponse HttpRequestResolver::resolve()
    {
		this->response_.http_version = "HTTP/1.1";

		//comprobar si la request es valida
		if (this->request_.is_valid() == false)
		{
			this->response_.status_code = 400;
		}
		else
		{
			this->location_ = this->resolve_location();
			this->file_path_ = this->location_.root + this->request_.uri.path;
			this->apply_method();
		}

		this->response_.status_msg = this->resolve_status_code();
		//buscar si se ha definido un archivo para ese codigo y rellenear el body con ello
		if (this->response_.status_code >= 400)
		{
			this->response_.body = "error";
			HttpHeaderContentLength *content_length_header = new HttpHeaderContentLength();
			content_length_header->set_value(this->response_.body.size());
			this->response_.headers.insert(content_length_header);
		}

        return this->response_;
    }
    
}
