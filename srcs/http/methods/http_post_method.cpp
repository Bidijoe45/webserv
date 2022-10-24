#include "http_post_method.hpp"
#include "../http_request.hpp"

#include <iostream>
namespace ws
{
    HttpPostMethod::HttpPostMethod(HttpRequest *request, const Location &location, const std::string &file_path, const ContentTypeMap &content_type_map)
        : HttpMethod(request, location, file_path, content_type_map) {}

    HttpResponse HttpPostMethod::execute()
    {
	    if (this->location_.upload_dir.size() == 0)
            throw HttpMethod::Exception(403);

        HttpHeaderMap::iterator header_map_it = this->request_->headers.find("content-type");
        if (header_map_it == this->request_->headers.end())
            throw HttpMethod::Exception(400);

	    HttpHeaderContentType *content_type = dynamic_cast<HttpHeaderContentType *>(header_map_it->second); 
	    if (content_type == NULL)
            throw HttpMethod::Exception(400);

	    if (content_type->content_type != "multipart/form-data")
            throw HttpMethod::Exception(403);

        std::map<std::string, std::string>::iterator boundary_param = content_type->parameters.find("boundary");
        if (boundary_param == content_type->parameters.end())
            throw HttpMethod::Exception(400);

	    HttpMultipartBodyParser multipart_parser(this->request_->body, boundary_param->second);
	    HttpMultipartBody multipart_body = multipart_parser.parse();

        if (!multipart_parser.is_valid())
            throw HttpMethod::Exception(400);

        std::vector<HttpMultipartBodyPart>::iterator it = multipart_body.parts.begin();
        std::vector<HttpMultipartBodyPart>::iterator ite = multipart_body.parts.end();

        while (it != ite)
        {
            HttpMultipartBodyPart part = *it;

            HttpHeaderMap::iterator header_cd_it = part.header_map.find("content-disposition");
            if (header_cd_it == part.header_map.end())
                throw HttpMethod::Exception(400);

            HttpHeaderContentDisposition * header_cd = dynamic_cast<HttpHeaderContentDisposition *>(header_cd_it->second);
            if (header_cd == NULL)
                throw HttpMethod::Exception(400);

            if (header_cd->filename.size() == 0)
            {
                it++;
                continue;
            }
        
            std::string file_path = this->location_.upload_dir + header_cd->filename;
            FileSystem upload_dir(this->location_.upload_dir);

            if (!upload_dir.is_valid() || !upload_dir.is_dir())
                throw HttpMethod::Exception(500);

            FileSystem file(file_path);

            if (!file.is_open())
                file.create(file_path);
            file.write(part.content);
            file.close();

            it++;
        }

        FileSystem file(this->file_path_);
        
	    if (!file.is_valid())
		    throw HttpMethod::Exception(201);

        if (file.is_dir() && this->location_.autoindex == true)
        {
            this->response_.body = this->generate_autoindex(file); 
            this->response_.generate_content_type_header("text/html; charset=utf-8");
        }
        else if (file.is_dir() && this->location_.autoindex == false)
        {
            throw HttpMethod::Exception(201);
        }
        else
        {
            std::string content_type = resolve_file_extension_content_type(file.get_file_extension());
            this->response_.body = file.get_content();
            this->response_.generate_content_type_header(content_type);
        }
        this->response_.generate_content_length_header();
        this->response_.status_code = 201;

        return this->response_;
    }

}
