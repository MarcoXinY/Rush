#pragma once
#include <string>
#include "RequestHeader.hpp"
#include "RequestLine.hpp"
#include "Response.hpp"

namespace http
{
class Response{
public:
    Response();
    Response(std::string const& sendMessage);
    //Response(RequestBody const& requestBody); todo ..
    const char * c_str();
private:
    RequestBody requestBody;
    RequestHeader requestHeader;
    RequestLine requestLine;

};
}

namespace http
{
class Response{
public:
    Response();
    Response(std::string const& sendMessage);
    //Response(RequestBody const& requestBody); todo ..
    const char * c_str();
    std::uint32_t size();
private:
    RequestBody requestBody{};
    RequestHeader requestHeader{};
    RequestLine requestLine{};
    std::uint32_t messageSize{0};

};
}