#pragma once
#include <string>
#include "json11.hpp"

using namespace json11;

namespace Service
{
class MessageDataItem{
public:
  std::string from;
  std::string text;
  std::string to;

  Json to_json() const;
};

class ClientSendToServerMessage {
public:
    int operate = 0;
    MessageDataItem messageDataItem;

    ClientSendToServerMessage();
    ClientSendToServerMessage(int, std::string, std::string, std::string);
    Json to_json() const;
    static void JsonStringToClass(std::string, ClientSendToServerMessage&);
};


class ServerSendClientMessage{
public:
  int code;
  std::string message;

  ServerSendClientMessage();
  ServerSendClientMessage(int, std::string);
  Json to_json() const;
  static void JsonStringToClass(std::string, ServerSendClientMessage&);
};
}//Service end