#pragma once

#include <string>
#include <vector>

namespace ws
{

  enum HTTP_HEADER_TYPE
  {
    HTTP_HEADER_UNKNOWN,
    HTTP_HEADER_ACCESS,
    HTTP_HEADER_HOST,
    HTTP_HEADER_ACCEPT
  };

  struct HttpHeader
  {
    virtual ~HttpHeader();
    HTTP_HEADER_TYPE type;
  };

  struct HttpHeaderUnknown : public HttpHeader {
    HttpHeaderUnknown();
    std::string value;
  };

  struct HttpHeaderAccept : public HttpHeader {
    HttpHeaderAccept();
    std::vector<std::string> list;
  };

}
