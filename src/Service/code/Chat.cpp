#include <iostream>
#include "Chat.hpp"
#include "JsonMessage.hpp"

namespace Service
{

Chat::Chat()
{
    std::cout << "creat a new chat Client" << std::endl;
}

bool Chat::getConnectedStatus() const
{
    return IsConnected;
}

void Chat::parseHandshake(unsigned char* input_frame, int input_len)
{
    webSocket.parseHandshake(input_frame, input_len);
    std::cout << "*****" << webSocket.params["localName"] << std::endl;
    std::cout << "*****" <<webSocket.params["peerName"] << std::endl;

    localName = webSocket.params["localName"];
    peerName = webSocket.params["peerName"];
}

std::string Chat::answerHandshake()
{
    std::string reponse = webSocket.answerHandshake();
    IsConnected = true;
    return reponse;
}

void Chat::processRequest(unsigned char* input_frame, int input_len)
{
    if(IsConnected == false)
    {
        //没有握手 执行握手
        parseHandshake(input_frame, input_len);
    }
    else
    {
        //握手完成 解析requestMessage
        int PayloadDataLenth = 0;
        unsigned char outBuffer[1024] = {0};
        webSocketLib::WebSocketFrameType frameType = webSocket.getFrame(input_frame, input_len, outBuffer, 1024, &PayloadDataLenth);
        std::string resqustMessage((char*)outBuffer, PayloadDataLenth);

        ClientSendToServerMessage receiveMessage;
        ClientSendToServerMessage::JsonStringToClass(resqustMessage, receiveMessage);

        std::cout << resqustMessage <<std::endl;
        std::cout << "******" <<std::endl;
        std::cout << receiveMessage.operate <<std::endl;
        std::cout << receiveMessage.messageDataItem.from <<std::endl;
        std::cout << receiveMessage.messageDataItem.text <<std::endl;
        std::cout << receiveMessage.messageDataItem.to <<std::endl;

    }
}

std::string Chat::responseWithProcessResult()
{
    if(IsConnected == false)
    {
        //没有握手 握手的response
        return answerHandshake();
    }
    else
    {
        //握手完成 回Response

        // std::string sendmsg = "abc";
        // unsigned char sendbuffer[1024] ={0};

        // int result = webSocket.makeFrame(webSocketLib::WebSocketFrameType{webSocketLib::WebSocketFrameType::TEXT_FRAME},
        //                                 (unsigned char*)sendmsg.c_str(),
        //                                 sendmsg.size(),
        //                                 sendbuffer,
        //                                 sizeof(sendbuffer));

        // std::string responseFrame((char *)sendbuffer, result);
        // std::cout << "result :" << result << std::endl;

        ServerSendClientMessage responseData(0,"successfully!");
        std::string responseDatastr =  Json(responseData).dump();
        unsigned char sendbuffer[1024] ={0};

        int result = webSocket.makeFrame(webSocketLib::WebSocketFrameType{webSocketLib::WebSocketFrameType::TEXT_FRAME},
                                        (unsigned char*)responseDatastr.c_str(),
                                        responseDatastr.size(),
                                        sendbuffer,
                                        sizeof(sendbuffer));

        std::string responseFrame((char *)sendbuffer, result);

        return responseFrame;
        // std::cout << "content :" << sendmsg << std::endl;
        //printf("Recv: %s, %d Bytes\n", sendbuffer, strlen((const char *)sendbuffer));
    }
}

Chat::~Chat()
{
    std::cout << "release a chat client " << std::endl;
}

}

