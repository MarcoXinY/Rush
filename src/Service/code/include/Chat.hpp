#pragma once
#include <string>
#include "WebSocket.hpp"
#include "JsonMessage.hpp"

namespace Service
{
class Chat
{
public:
Chat();
bool getConnectedStatus() const;
void processRequest(unsigned char* input_frame, int input_len);
std::string responseWithProcessResult(int code, std::string message);
std::string MessageForForwardingOtherClient();

~Chat();

private:
void parseHandshake(unsigned char* input_frame, int input_len);
std::string answerHandshake();

public:
ClientSendToServerMessage receiveMessage;
std::string localName;
std::string peerName;
private:
webSocketLib::WebSocket webSocket;
bool IsConnected = false;

};
}//namespace Service