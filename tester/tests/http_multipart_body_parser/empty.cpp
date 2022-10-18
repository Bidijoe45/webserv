#include "../../../srcs/http/http_multipart_body.hpp"
#include "../../../srcs/http/http_multipart_body_parser.hpp"

int main()
{
    std::string raw_body = "";
    std::string boundary = "";
    ws::HttpMultipartBodyParser multipart_parser(raw_body, boundary);
    ws::HttpMultipartBody parsed_body = multipart_parser.parse();

    if (multipart_parser.is_valid())
        return 1;

    return 0;
}
