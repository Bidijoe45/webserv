#include "../../../srcs/http/http_multipart_body.hpp"
#include "../../../srcs/http/http_multipart_body_parser.hpp"

std::string prepare_raw_body()
{
    std::string raw_body;

    raw_body = 
    "--boundary\r\n"
    "Content-Disposition: form-data; name=\"datafile1\"; filename=\"file1.txt\"\r\n"
    "Content-Type: text/plain\r\n\r\n"
    "file 1 content"
    "--boundary\r\n"
    "Content-Disposition: form-data; name=\"datafile2\"; filename=\"file2.txt\"\r\n"
    "Content-Type: image/gif\r\n\r\n"
    "file 2 content"
    "--boundary\r\n"
    "Content-Disposition: form-data; name=\"datafile3\"; filename=\"file3.gif\"\r\n"
    "Content-Type: image/gif\r\n\r\n"
    "file 3 content"
    "--boundary\r\n";

    return raw_body;
}

int main()
{
    std::string raw_body = prepare_raw_body();
    std::string boundary = "boundary";
    ws::HttpMultipartBodyParser multipart_parser(raw_body, boundary);
    ws::HttpMultipartBody parsed_body = multipart_parser.parse();

    if (multipart_parser.is_valid())
        return 1;

    return 0;
}
