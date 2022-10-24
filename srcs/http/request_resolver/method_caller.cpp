#include "method_caller.hpp"

#include "../methods/http_get_method.hpp"
#include "../methods/http_post_method.hpp"
#include "../methods/http_delete_method.hpp"

namespace ws
{

    MethodCaller::MethodCaller(HttpRequest *request, const ContentTypeMap &content_type_map)
        : RequestHandler(), request_(request), content_type_map_(content_type_map) {}

    RequestHandlerPayload *MethodCaller::handle(RequestHandlerPayload *payload)
    {
        try {
            switch (this->request_->request_line.method)
            {
                case HTTP_METHOD_GET:
                    payload->response = HttpGetMethod(this->request_, payload->location, payload->file_path, this->content_type_map_).execute();
                    break;
                case HTTP_METHOD_POST:
                    payload->response = HttpPostMethod(this->request_, payload->location, payload->file_path, this->content_type_map_).execute();
                    break;
                case HTTP_METHOD_DELETE:
                    payload->response = HttpDeleteMethod(this->request_, payload->location, payload->file_path, this->content_type_map_).execute();
                    break;
                default:
                    throw RequestHandler::Exception(501);
            }
        }
        catch (HttpMethod::Exception &e)
        {
            throw RequestHandler::Exception(e.get_error_code());
        }

        return RequestHandler::handle(payload);
    }
}
