#pragma once
#include <string>
namespace http
{

class RequestHeader
{
public:
    enum HeaderOption{//以后会加新的header选项
        ContentType,
        ContentLength
    };
    using headerMap = std::multimap<std::string, std::string> HeaderMap;
    RequestHeader();
    void AddHeaderOption(HeaderOption option,std::string cosnt& content);
    std::string ToString();
private:
    HeaderMap headerMap{};
}

}
