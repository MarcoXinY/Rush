#include "RequestHeader.hpp"

namespace http 
{
RequestHeader::RequestHeader()
{
}
void RequestHeader::AddHeaderOption(HeaderOption option,std::string cosnt& content)
{
    swith(option)
    {
        case HeaderOption::ContentType:
            headerMap.emplace("Content-Type",content);
            break;
        case HeaderOption::ContentLength:
            headerMap.emplace("Content-Length",content);
            break;
        default:
            break;
    }
}

std::string RequestHeader::ToString()
{
    std::string headerString{};

    for(auto headerOption :headerMap)
    {
        headerString += headerMap->first +": "+ headerMap->second + "\r\n";
    }
    return headerString;
}

}