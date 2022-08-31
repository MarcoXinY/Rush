#pragma once
#include <string>
#include "RequsetBody.hpp"

namespace http
{
RequestBody::RequestBody()
{
}
RequestBody::RequestBody(std::string bodyContent)
    :bodyContent{bodyContent}
{
}
std::string RequestBody::ToString()
{
    return "\r\n<html><div><h1>"+bodyContent+"</h1></div></html>\r\n";
}

}