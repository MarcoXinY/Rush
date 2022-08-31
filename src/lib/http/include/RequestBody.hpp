#pragma once
#include <string>
namespace http
{
class RequestBody
{
public:
    RequestBody();
    RequestBody(const std::string& bodyContent);
    std::string ToString();
private:
    std::string bodyContent{"Hello World!"};
};
}
