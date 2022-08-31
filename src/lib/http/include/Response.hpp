#pragma once
#include <string>
#include "RequestHeader.hpp"
#include "RequestLine.hpp"
#include "RequestBody.hpp"

namespace http
{
class Response{
public:
    Response();
    Response(std::string const& sendMessage);
    //Response(RequestBody const& requestBody); todo ..
    std::string ToString();
private:
    RequestBody requestBody{};
    RequestHeader requestHeader{};
    RequestLine requestLine{};

};
}