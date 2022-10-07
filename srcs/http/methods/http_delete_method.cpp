#include "http_delete_method.hpp"

namespace ws
{
    HttpDeleteMethod::HttpDeleteMethod(HttpRequest *request, const Location &location, const std::string &file_path, const ContentTypeMap &content_type_map)
        : HttpMethod(request, location, file_path, content_type_map) {}

    HttpResponse HttpDeleteMethod::execute()
    {

	    if (this->location_.upload_dir.size() == 0)
	        throw HttpMethod::Exception(403);

	    FileSystem file(this->file_path_);
	    if (!file.is_valid() || file.is_dir())
	        throw HttpMethod::Exception(403);
	
	    bool removed = file.remove();
        
        if (!removed)
            throw HttpMethod::Exception(500);

        file.close();

        throw HttpMethod::Exception(200);

        return this->response_;
    }
}
