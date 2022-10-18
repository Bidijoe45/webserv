#include "../../../srcs/http/http_multipart_body.hpp"
#include "../../../srcs/http/http_multipart_body_parser.hpp"
#include "../../../srcs/http/http_header_parser.hpp"
#include "../../../srcs/http/http_header_map.hpp"

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
    "--boundary--\r\n";

    return raw_body;
}

ws::HttpMultipartBody prepare_expected_body()
{
    ws::HttpMultipartBody body;

    ws::HttpMultipartBodyPart part1;
    std::vector<std::string> headers1;
    headers1.push_back("Content-Disposition: form-data; name=\"datafile1\"; filename=\"file1.txt\"");
    headers1.push_back("Content-Type: text/plain");
    ws::HttpHeaderParser parser1(headers1);
    ws::HttpHeaderMap header_map1 = parser1.parse_block();
    part1.header_map = header_map1;
    part1.content = "file 1 content";

    ws::HttpMultipartBodyPart part2;
    std::vector<std::string> headers2;
    headers2.push_back("Content-Disposition: form-data; name=\"datafile2\"; filename=\"file2.txt\"");
    headers2.push_back("Content-Type: image/gif");
    ws::HttpHeaderParser parser2(headers2);
    ws::HttpHeaderMap header_map2 = parser2.parse_block();
    part2.header_map = header_map2;
    part2.content = "file 2 content";

    ws::HttpMultipartBodyPart part3;
    std::vector<std::string> headers3;
    headers3.push_back("Content-Disposition: form-data; name=\"datafile3\"; filename=\"file3.gif\"");
    headers3.push_back("Content-Type: image/gif");
    ws::HttpHeaderParser parser3(headers3);
    ws::HttpHeaderMap header_map3 = parser3.parse_block();
    part3.header_map = header_map3;
    part3.content = "file 3 content";

    body.parts.push_back(part1);
    body.parts.push_back(part2);
    body.parts.push_back(part3);

    return body;
}

int main()
{
    std::string raw_body = prepare_raw_body();
    std::string boundary = "boundary";
    ws::HttpMultipartBodyParser multipart_parser(raw_body, boundary);
    ws::HttpMultipartBody parsed_body = multipart_parser.parse();
    ws::HttpMultipartBody expected_body = prepare_expected_body();

    if (parsed_body == expected_body)
        return 0;

    return 1;
}
