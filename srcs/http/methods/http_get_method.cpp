#include "http_get_method.hpp"

namespace ws
{
    HttpGetMethod::HttpGetMethod(HttpRequest *request, const Location &location, const std::string &file_path, const ContentTypeMap &content_type_map)
        : HttpMethod(request, location, file_path, content_type_map) {}

    HttpResponse HttpGetMethod::execute()
    {
        FileSystem file(this->file_path_);
        
		if (!file.is_valid())
		    throw HttpMethod::Exception(404);

        if (file.is_dir() && this->location_.autoindex == true)
        {
            this->response_.body = this->generate_autoindex(file); 
            this->response_.generate_content_type_header("text/html");
        }
        else if (file.is_dir() && this->location_.autoindex == false)
        {
            throw HttpMethod::Exception(403);
        }
        else
        {
            std::string content_type = resolve_file_extension_content_type(file.get_file_extension());
            this->response_.body = file.get_content();
            this->response_.generate_content_type_header(content_type);
        }

        this->response_.generate_content_length_header();
        this->response_.status_code = 200;

        return this->response_;
    }
}
