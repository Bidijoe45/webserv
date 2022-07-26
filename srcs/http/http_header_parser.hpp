#pragma once

#include "http_header.hpp"

namespace ws
{

  class HttpHeaderParser
  {
    public:
      HttpHeaderParser();
      HttpHeader *parse(std::string name, std::string value);
      
  };

}
