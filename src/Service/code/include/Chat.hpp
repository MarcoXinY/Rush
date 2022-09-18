#pragma once
#include <string>
#include "WebSocket.hpp"

namespace Service
{
class Chat
{
public:
Chat();
bool getConnectedStatus() const;
void processRequest(unsigned char* input_frame, int input_len);
std::string responseWithProcessResult();

~Chat();

private:
void parseHandshake(unsigned char* input_frame, int input_len);
std::string answerHandshake();

private:
std::string localName;
std::string peerName;
webSocketLib::WebSocket webSocket;
bool IsConnected = false;

};
}//namespace Service