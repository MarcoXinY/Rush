#include <string>
#include "Response.hpp"

namespace http
{

Response::Response()
{
    //添加header
    requestHeader.AddHeaderOption(HeaderOption::ContentLength, std::to_string(requestBody.ToString().size()));
    requestHeader.AddHeaderOption(HeaderOption::ContentType, "text/html\ncharset: utf-8");
    
}

Response::Response(std::string const& sendMessage)
: requestBody{sendMessage}
{
    //添加header
    requestHeader.AddHeaderOption(HeaderOption::ContentLength, std::to_string(requestBody.ToString().size()));
    requestHeader.AddHeaderOption(HeaderOption::ContentType, "text/html\ncharset: utf-8");
}

//Response(RequestBody const& requestBody); todo ..
std::string Response::ToString()
{
    //测试 发送成功的报文
    return requestLine.ToString() + requestHeader.ToString() + requestBody.ToString();
}
}