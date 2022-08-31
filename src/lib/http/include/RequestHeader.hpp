#pragma once
#include <string>
#include <map>

namespace http
{

enum class HeaderOption{//以后会加新的header选项
    ContentType,
    ContentLength
};

class RequestHeader
{
public:
    using HeaderMap = std::map<std::string, std::string>;
    RequestHeader();
    void AddHeaderOption(HeaderOption option, const std::string& content);
    std::string ToString();
private:
    HeaderMap headerMap{};
};

}
