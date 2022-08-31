#pragma once
#include "RequestHeader.hpp"

namespace http
{
class Response{
public:
    Response::Response()
    {
        //添加header
        requestHeader.AddHeaderOption(RequestHeader::HeaderOption::ContentType, "text/html\ncharset: gbk");
        requestHeader.AddHeaderOption(RequestHeader::HeaderOption::ContentLength, to_string(requestBody.ToString().size()));
    }
    Response::Response(std::string const& sendMessage)
    : requestBody{sendMessage}
    {
        //添加header
        requestHeader.AddHeaderOption(RequestHeader::HeaderOption::ContentType, "text/html\ncharset: gbk");
        requestHeader.AddHeaderOption(RequestHeader::HeaderOption::ContentLength, to_string(requestBody.ToString().size()));
    }
    //Response(RequestBody const& requestBody); todo ..
    const char * Response::c_str()
    {
        //测试 发送成功的报文
        httpString = requestLine.ToString() + requestHeader.ToString() + requestBody.ToString();
        return httpString.c_str();
    }
    std::uint32_t Response::size()
    {
        return (requestLine.ToString() + requestHeader.ToString() + requestBody.ToString()).size();
    }

};
}