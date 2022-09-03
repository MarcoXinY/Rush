#include <string>
#include "RequestLine.hpp"
namespace http
{
    RequestLine::RequestLine()
    {
    }
    std::string RequestLine::ToString()
    {
        switch(statusCode)
        {//目前只是写了一部分的状态码 todo..
            case Staus::INVALID_CODE:
                return httpVersion + " 0 " + "Invalid Code" + "\r\n";
            case Staus::OK:
                return httpVersion + " 200 " + "OK" + "\r\n";
            case Staus::BAD_REQUEST:
                return httpVersion + " 400 " + "Bad Request" + "\r\n";
            case Staus::NOT_FOUND:
                return httpVersion + " 404 " + "Not Found" + "\r\n";
            case VERSION_NOT_SUPPORTED:
                return httpVersion + " 505 " + "Version Not Supported" + "\r\n";
        }
    }
}
