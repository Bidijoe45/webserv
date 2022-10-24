#include "file_path_resolver.hpp"

namespace ws
{
    FilePathResolver::FilePathResolver(HttpRequest *request) : RequestHandler(), request_(request) {}

    RequestHandlerPayload *FilePathResolver::handle(RequestHandlerPayload *payload)
    {
        std::string file_path = this->get_file_path(payload);
        FileSystem root_dir(payload->location.root);

        if (!root_dir.is_valid() || !root_dir.is_dir())
            throw RequestHandler::Exception(500);

        FileSystem file(file_path);

        if (file.is_dir() && payload->location.index.size() > 0)
        {
            std::string index_path = file_path + payload->location.index; 
            FileSystem index_file(index_path);

            if (index_file.is_valid() && !index_file.is_dir())
                file_path = index_path;
        }
    
        payload->file_path = file_path;
        return RequestHandler::handle(payload);
    }

    std::string FilePathResolver::get_file_path(RequestHandlerPayload *payload)
    {
        std::string uri_path = this->request_->request_line.uri.path;
        std::string new_uri_path; 
        if (uri_path.compare(0, payload->location.path.size(), payload->location.path) == 0)
            new_uri_path = this->request_->request_line.uri.path.substr(payload->location.path.size());
		return payload->location.root + new_uri_path;
    }
}
