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


    HttpResponse HttpRequestResolver::resolve()
    {
        HttpResponse response;
		// rellenar response.version
		// is request valid ?
		// is request version http1.1 ?
        Location location = this->resolve_location();
        std::string file_path = location.root + this->request_.uri.path;

		//Aplicar metodo de la request al target (y comprobar si es valido)
		
		// si el status_code es de error, sustituir el file_path por la pagina de error
		// rellenar response body si hace falta
		// rellenar header content-length e insertar en el mapa de la response
	
		// resolve status_code to find the status_msg of the response
	


        response.version = "HTTP/1.1";
        response.status_code = 200;
        response.status_msg = "OK";

        response.body = "Hello Worla!";
		HttpHeaderContentLength *content_length = new HttpHeaderContentLength();
		content_length->set_value(response.body.size());
		this->response_.headers.insert(content_length);

        return response;
    }
    
}
