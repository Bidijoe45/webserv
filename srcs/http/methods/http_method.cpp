#include "http_method.hpp"

namespace ws
{
    HttpMethod::HttpMethod(HttpRequest *request, const Location &location, const std::string &file_path, const ContentTypeMap &content_type_map)
        : request_(request), location_(location), file_path_(file_path), content_type_map_(content_type_map) {}

    HttpMethod::Exception::Exception(size_t error_code) : error_code_(error_code) {}

    const char *HttpMethod::Exception::what()
    {
        return "Http Method Exception";
    }

    size_t HttpMethod::Exception::get_error_code()
    {
        return this->error_code_;
    }

    std::string HttpMethod::resolve_file_extension_content_type(std::string file_extension)
    {
        std::string content_type = "application/octet-stream";
        
		ContentTypeMap::iterator type_it = this->content_type_map_.find(file_extension);
		if (type_it != this->content_type_map_.end())
			content_type = type_it->second;

		return content_type;
    }

    std::string HttpMethod::generate_autoindex(const FileSystem &file)
    {
        std::string autoindex_html;
        std::vector<std::string> dir_files = file.read_dir();
            
        autoindex_html.append("<html>");
        autoindex_html.append("<head>");
        autoindex_html.append("<title>");
        autoindex_html.append("Index of " + this->request_->request_line.uri.path);
        autoindex_html.append("</title>");
        autoindex_html.append("</head>");
        autoindex_html.append("<body>");
        autoindex_html.append("<h1>");
        autoindex_html.append("Index of " + this->request_->request_line.uri.path);
        autoindex_html.append("</h1>");
        autoindex_html.append("<hr>");

        for (size_t i=0; i < dir_files.size(); i++)
        {
            autoindex_html.append("<a href=\"");
            autoindex_html.append(this->request_->request_line.uri.path);
            if (this->request_->request_line.uri.path.back() != '/')
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

    HttpResponse HttpMethod::get_response()
    {
        return this->response_;
    }

}
