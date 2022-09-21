#include <iostream>
#include <sstream>
#include "JsonMessage.hpp"

namespace Service
{
Json MessageDataItem::to_json() const { return Json::object ({
    {"from", from},
    {"text", text},
    {"to"  , to}
});}

ClientSendToServerMessage::ClientSendToServerMessage()
{
    operate = 0;
    messageDataItem.from = "unknown";
    messageDataItem.text = "empty";
    messageDataItem.to = "unknown";
}

ClientSendToServerMessage::ClientSendToServerMessage(int operate_, 
        std::string from, std::string text, std::string to)
{
    operate = operate_;
    messageDataItem.from = from;
    messageDataItem.text = text;
    messageDataItem.to = to;
}

Json ClientSendToServerMessage::to_json() const 
{ 
    return Json::object ({
    {"operate", operate},
    {"data", Json(messageDataItem)}});
}

void ClientSendToServerMessage::JsonStringToClass(std::string jsonString, ClientSendToServerMessage& message)
{
    std::cout << "enter JsonStringToClass: " << std::endl;
    std::cout << jsonString << std::endl;

    std::string err;
    const auto json = Json::parse(jsonString, err);

    if (!err.empty())
        std::cout <<"Josn parse Failed: "<< err << std::endl;

    std::stringstream stream;
    stream << json["operate"].dump();
    stream >> message.operate;

    std::cout << "parse json data: " << std::endl;
    std::cout << json["data"]["from"].string_value()  << std::endl;
    std::cout << json["data"]["text"].string_value() << std::endl;
    std::cout << json["data"]["to"].string_value() << std::endl;

    message.messageDataItem.from = json["data"]["from"].string_value() ;
    message.messageDataItem.text = json["data"]["text"].string_value();
    message.messageDataItem.to = json["data"]["to"].string_value();

    return;
}

ServerSendClientMessage::ServerSendClientMessage()
{
    code = 0;
    message = "";
}

ServerSendClientMessage::ServerSendClientMessage(int code_, std::string message_)
{
    code = code_;
    message = message_;
}

Json ServerSendClientMessage::to_json() const 
{ 
    return Json::object ({
    {"code", code},
    {"message", message}});
}

void ServerSendClientMessage::JsonStringToClass(std::string jsonString ,ServerSendClientMessage& message)
{
    std::string err;
    const auto json = Json::parse(jsonString, err);

    std::stringstream stream;
    stream << json["code"].dump();
    stream >> message.code;
    message.message = json["message"].string_value();
    return;
}
}//Service end 