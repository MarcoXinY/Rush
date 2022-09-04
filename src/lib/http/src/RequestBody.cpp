#include <string>
#include "RequestBody.hpp"

namespace http
{
RequestBody::RequestBody()
{
}
RequestBody::RequestBody(const std::string& bodyContent)
    :bodyContent{bodyContent}
{
}
std::string RequestBody::ToString()
{
    return bodyContent;
}

}